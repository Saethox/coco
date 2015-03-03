#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include <jni.h>

#include "coco.h"
#include "coco.c"
#include "JNIinterface.h"

/*
 * Class:     JNIinterface
 * Method:    cocoGetProblem
 * Signature: (Ljava/lang/String;I)J
 */
JNIEXPORT jlong JNICALL Java_JNIinterface_cocoGetProblem
(JNIEnv *jenv, jclass interface_cls, jstring jproblem_suit, jint jfunction_index) {
    coco_problem_t *pb = NULL;
    const char *problem_suit;
    int function_index;
    if (interface_cls == NULL)
        printf("Null interface_cls found\n");
    problem_suit = (*jenv)->GetStringUTFChars(jenv, jproblem_suit, NULL);
    function_index = (int)jfunction_index;
    pb = coco_get_problem(problem_suit, function_index);
    return (jlong)pb; /* or long? */
    
}

/*
 * Class:     JNIinterface
 * Method:    cocoObserveProblem
 * Signature: (Ljava/lang/String;JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_JNIinterface_cocoObserveProblem
(JNIEnv *, jclass interface_cls, jstring jobserver, jlong jproblem, jstring joptions) {
    coco_problem_t *pb = NULL;
    const char *observer;
    const char *options;
    if (interface_cls == NULL)
        printf("Null interface_cls found\n");
    pb = (coco_problem_t *)jproblem;
    observer = (*jenv)->GetStringUTFChars(jenv, jobserver, NULL);
    options = (*jenv)->GetStringUTFChars(jenv, joptions, NULL);
    pb = coco_observe_problem(observer, pb, options);
    return (jlong)pb; /* or long? */
}

/*
 * Class:     JNIinterface
 * Method:    cocoFreeProblem
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_JNIinterface_cocoFreeProblem
(JNIEnv *, jclass interface_cls, jlong jproblem) {
    coco_problem_t *pb = NULL;
    if (interface_cls == NULL)
        printf("Null interface_cls found\n");
    pb = (coco_problem_t *)jproblem;
    coco_free_problem(pb);
}

/*
 * Class:     JNIinterface
 * Method:    cocoEvaluateFunction
 * Signature: (LProblem;[D)[D
 */
JNIEXPORT jdoubleArray JNICALL Java_JNIinterface_cocoEvaluateFunction
(JNIEnv *jenv, jclass interface_cls, jobject problem, jdoubleArray x) {

	double *y; /* Result of evaluation. To be allocated with coco_allocate_vector(coco_get_number_of_objectives(pb)) */
	coco_problem_t *pb = NULL; /* Will contain the C problem */
    
	jint nb_objectives;
    jclass cls;
	jfieldID fid;
	jlong jproblem;
	jdouble *cx;
	jdoubleArray jy; /* Returned double array */

	/* This test is both to prevent warning because interface_cls was not used and check exceptions */
	if (interface_cls == NULL)
		printf("Null interface_cls found\n");
    
    /* Get attributes from jobject problem */
    cls = (*jenv)->GetObjectClass(jenv, problem);
    if (cls == NULL)
        printf("Null cls\n");

	/* Get Problem.problem */
	fid = (*jenv)->GetFieldID(jenv, cls, "problem", "J");
	if(fid == NULL)
		printf("Null fid\n");
	jproblem = (*jenv)->GetLongField(jenv, problem, fid);
	/* Cast it to coco_problem_t */
    pb = (coco_problem_t *)jproblem;

	/* Call coco_evaluate_function */
	cx = (*jenv)->GetDoubleArrayElements(jenv, x, NULL);
    fid = (*jenv)->GetFieldID(jenv, cls, "number_of_objectives", "I");
    if(fid == NULL)
        printf("Null fid2\n");
    nb_objectives = (*jenv)->GetIntField(jenv, problem, fid);
	y = coco_allocate_vector(nb_objectives);
	coco_evaluate_function(pb, cx, y);

	/* Prepare the return value */
	jy = (*jenv)->NewDoubleArray(jenv, nb_objectives);
	(*jenv)->SetDoubleArrayRegion(jenv, jy, 0, nb_objectives, y);

	/* Free resources */
	coco_free_memory(y);
	(*jenv)->ReleaseDoubleArrayElements(jenv, x, cx, JNI_ABORT);
	return jy;
}

/*
 * Class:     JNIinterface
 * Method:    cocoGetNumberOfVariables
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_JNIinterface_cocoGetNumberOfVariables
(JNIEnv *jenv, jclass interface_cls, jlong problem) {

	coco_problem_t *pb = NULL;
	const char *problem_suit;
	int function_index;
	int res;

	jfieldID fid;
	jstring jproblem_suit;
	jint jfunction_index;
    jclass cls;

	/* This test is both to prevent warning because interface_cls was not used and check exceptions */
	if (interface_cls == NULL)
		printf("Null interface_cls found\n");
    
    /* Get attributes from jobject problem */
    cls = (*jenv)->GetObjectClass(jenv, problem);
    if (cls == NULL)
        printf("Null cls\n");

	/* Get problem_suit */
	fid = (*jenv)->GetFieldID(jenv, cls, "problem_suit", "Ljava/lang/String;");
	if(fid == NULL)
		printf("Null fid\n");
	jproblem_suit = (*jenv)->GetObjectField(jenv, problem, fid);
	problem_suit = (*jenv)->GetStringUTFChars(jenv, jproblem_suit, NULL);

	/* Get function_index */
	fid = (*jenv)->GetFieldID(jenv, cls, "function_index", "I");
	if(fid == NULL)
		printf("Null fid2\n");
	jfunction_index = (*jenv)->GetIntField(jenv, problem, fid);
	function_index = (int)jfunction_index; /* conversion not necessary */

	pb = coco_get_problem(problem_suit, function_index);
	res = coco_get_number_of_variables(pb);

	coco_free_problem(pb);
	(*jenv)->ReleaseStringUTFChars(jenv, jproblem_suit, problem_suit);

	return res;
}

/*
 * Class:     JNIinterface
 * Method:    cocoGetNumberOfObjectives
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_JNIinterface_cocoGetNumberOfObjectives
(JNIEnv *jenv, jclass interface_cls, jlong problem) {
	coco_problem_t *pb = NULL;
	const char *problem_suit;
	int function_index;
	int res;

	jfieldID fid;
	jstring jproblem_suit;
	jint jfunction_index;
    jclass cls;

	/* This test is both to prevent warning because interface_cls was not used and check exceptions */
	if (interface_cls == NULL)
		printf("Null interface_cls found\n");
    
    /* Get attributes from jobject problem */
    cls = (*jenv)->GetObjectClass(jenv, problem);
    if (cls == NULL)
        printf("Null cls\n");

	/* Get problem_suit */
	fid = (*jenv)->GetFieldID(jenv, cls, "problem_suit", "Ljava/lang/String;");
	if(fid == NULL)
		printf("Null fid\n");
	jproblem_suit = (*jenv)->GetObjectField(jenv, problem, fid);
	problem_suit = (*jenv)->GetStringUTFChars(jenv, jproblem_suit, NULL);

	/* Get function_index */
	fid = (*jenv)->GetFieldID(jenv, cls, "function_index", "I");
	if(fid == NULL)
		printf("Null fid2\n");
	jfunction_index = (*jenv)->GetIntField(jenv, problem, fid);
	function_index = (int)jfunction_index;

	pb = coco_get_problem(problem_suit, function_index);
	res = coco_get_number_of_objectives(pb);

	coco_free_problem(pb);
	(*jenv)->ReleaseStringUTFChars(jenv, jproblem_suit, problem_suit);

	return res;

}

/*
 * Class:     JNIinterface
 * Method:    cocoGetSmallestValuesOfInterest
 * Signature: (J)[D
 */
JNIEXPORT jdoubleArray JNICALL Java_JNIinterface_cocoGetSmallestValuesOfInterest
(JNIEnv *jenv, jclass interface_cls, jlong problem) {
	const double *cres;
	coco_problem_t *pb = NULL;

	const char *problem_suit;
	int function_index;
	int nb_variables;

	jfieldID fid;
	jstring jproblem_suit;
	jint jfunction_index;
	jdoubleArray res;
    jclass cls;

	/* This test is both to prevent warning because interface_cls was not used and check exceptions */
	if (interface_cls == NULL)
		printf("Null interface_cls found\n");
    
    /* Get attributes from jobject problem */
    cls = (*jenv)->GetObjectClass(jenv, problem);
    if (cls == NULL)
        printf("Null cls\n");

	/* Get problem_suit */
	fid = (*jenv)->GetFieldID(jenv, cls, "problem_suit", "Ljava/lang/String;");
	if(fid == NULL)
		printf("Null fid\n");
	jproblem_suit = (*jenv)->GetObjectField(jenv, problem, fid);
	problem_suit = (*jenv)->GetStringUTFChars(jenv, jproblem_suit, NULL);

	/* Get function_index */
	fid = (*jenv)->GetFieldID(jenv, cls, "function_index", "I");
	if(fid == NULL)
		printf("Null fid2\n");
	jfunction_index = (*jenv)->GetIntField(jenv, problem, fid);
	function_index = (int)jfunction_index;

	pb = coco_get_problem(problem_suit, function_index);
	cres = coco_get_smallest_values_of_interest(pb);
	nb_variables = coco_get_number_of_variables(pb);

	/* Prepare the return value */
	res = (*jenv)->NewDoubleArray(jenv, nb_variables);
	(*jenv)->SetDoubleArrayRegion(jenv, res, 0, nb_variables, cres);

	coco_free_problem(pb);
	(*jenv)->ReleaseStringUTFChars(jenv, jproblem_suit, problem_suit);

	return res;
}

/*
 * Class:     JNIinterface
 * Method:    cocoGetLargestValuesOfInterest
 * Signature: (J)[D
 */
JNIEXPORT jdoubleArray JNICALL Java_JNIinterface_cocoGetLargestValuesOfInterest
(JNIEnv *jenv, jclass interface_cls, jlong problem) {
	const double *cres;
	coco_problem_t *pb = NULL;

	const char *problem_suit;
	int function_index;
	int nb_variables;

	jfieldID fid;
	jstring jproblem_suit;
	jint jfunction_index;
	jdoubleArray res;
    jclass cls;

	/* This test is both to prevent warning because interface_cls was not used and check exceptions */
	if (interface_cls == NULL)
		printf("Null interface_cls found\n");
    
    /* Get attributes from jobject problem */
    cls = (*jenv)->GetObjectClass(jenv, problem);
    if (cls == NULL)
        printf("Null cls\n");

	/* Get problem_suit */
	fid = (*jenv)->GetFieldID(jenv, cls, "problem_suit", "Ljava/lang/String;");
	if(fid == NULL)
		printf("Null fid\n");
	jproblem_suit = (*jenv)->GetObjectField(jenv, problem, fid);
	problem_suit = (*jenv)->GetStringUTFChars(jenv, jproblem_suit, NULL);

	/* Get function_index */
	fid = (*jenv)->GetFieldID(jenv, cls, "function_index", "I");
	if(fid == NULL)
		printf("Null fid2\n");
	jfunction_index = (*jenv)->GetIntField(jenv, problem, fid);
	function_index = (int)jfunction_index;

	pb = coco_get_problem(problem_suit, function_index);
	cres = coco_get_largest_values_of_interest(pb);
	nb_variables = coco_get_number_of_variables(pb);

	/* Prepare the return value */
	res = (*jenv)->NewDoubleArray(jenv, nb_variables);
	(*jenv)->SetDoubleArrayRegion(jenv, res, 0, nb_variables, cres);

	coco_free_problem(pb);
	(*jenv)->ReleaseStringUTFChars(jenv, jproblem_suit, problem_suit);

	return res;
}

/*
 * Class:     JNIinterface
 * Method:    validProblem
 * Signature: (Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Java_JNIinterface_validProblem
(JNIEnv *jenv, jclass interface_cls, jstring suit, jint findex) {
    coco_problem_t *pb = NULL;
    const char *problem_suit;
    
    /* This test is both to prevent warning because interface_cls was not used and check exceptions */
    if (interface_cls == NULL)
        printf("Null interface_cls found\n");
    
    problem_suit = (*jenv)->GetStringUTFChars(jenv, suit, NULL);
    pb = coco_get_problem(problem_suit, findex);
    (*jenv)->ReleaseStringUTFChars(jenv, suit, problem_suit);
    if (pb == NULL)
        return JNI_FALSE;
    else
        coco_free_problem(pb);
        return JNI_TRUE;
    
}

/*
 * Class:     JNIinterface
 * Method:    cocoGetProblemId
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_JNIinterface_cocoGetProblemId
(JNIEnv *jenv, jclass interface_cls, jlong problem) {
    coco_problem_t *pb = NULL;
    const char *res;
    jstring jres;
    if (interface_cls == NULL)
        printf("Null interface_cls found\n");
    pb = (coco_problem_t *)jproblem;
    res = coco_get_problem_id(pb);
    jres = (*jenv)->NewStringUTF(jenv, res);
    return jres;
}

/*
 * Class:     JNIinterface
 * Method:    cocoGetProblemName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_JNIinterface_cocoGetProblemName
(JNIEnv *jenv, jclass interface_cls, jlong jproblem) {
    coco_problem_t *pb = NULL;
    const char *res;
    jstring jres;
    if (interface_cls == NULL)
        printf("Null interface_cls found\n");
    pb = (coco_problem_t *)jproblem;
    res = coco_get_problem_name(pb);
    jres = (*jenv)->NewStringUTF(jenv, res);
    return jres;
}

