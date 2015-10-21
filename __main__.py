#!/usr/bin/env python
"""``python bbob_pproc`` tests the package bbob_pproc and should run through 
smoothly from a system command shell. It however depends on data files that 
might not be available (to be improved). 

This test can and should become much more sophisticated.  

"""

import os, sys, time
import fnmatch

# Add the path to bbob_pproc
if __name__ == "__main__":
    (filepath, filename) = os.path.split(sys.argv[0])
    sys.path.append(os.path.join(filepath, os.path.pardir))

import doctest

def join_path(a, *p):
    path = os.path.join(a, *p)
    return path

def process_doctest_output(stream=None):
    """ """
    import fileinput
    s1 = ""
    s2 = ""
    s3 = ""
    state = 0
    for line in fileinput.input(stream):  # takes argv as file or stdin
        if 1 < 3:
            s3 += line
            if state < -1 and line.startswith('***'):
                print(s3)
            if line.startswith('***'):
                s3 = ""
            
        if state == -1:  # found a failed example line
            s1 += '\n\n*** Failed Example:' + line
            s2 += '\n\n\n'   # line
            # state = 0  # wait for 'Expected:' line 
            
        if line.startswith('Expected:'):
            state = 1
            continue
        elif line.startswith('Got:'):
            state = 2
            continue
        elif line.startswith('***'):  # marks end of failed example
            state = 0
        elif line.startswith('Failed example:'):
            state = -1
        elif line.startswith('Exception raised'):
            state = -2

        # in effect more else:
        if state == 1:
            s1 += line + ''
        if state == 2:
            s2 += line + ''
    
if __name__ == "__main__": 
    """these tests are executed when ``python bbob_pproc`` is called.  

    with ``wine`` as second argument ``C:\\Python26\\python.exe`` 
    instead of ``python`` is called
    
    """

    python = 'python '  # how to call python 
    if len(sys.argv) > 1 and sys.argv[1] == 'wine':
        python = 'C:\\Python26\\python.exe ' # works for wine
    
    data_path = join_path(' ..', '..', 'data-archive', 'data')

    command = join_path(' bbob_pproc', 'rungeneric.py ')
    
    print '*** testing module bbob_pproc ***'
    t0 = time.time()
    print time.asctime()
    os.system(python + command + # ' --omit-single ' +
                join_path(data_path, 'gecco-bbob-1-24', '2010', 'data', 'IPOP-ACTCMA-ES ') +
                # join_path(data_path, 'gecco-bbob-1-24', '2010', 'data', '1komma4mirser ') +
                join_path(data_path, 'gecco-bbob-1-24', '2009', 'data', 'MCS ') +
                join_path(data_path, 'gecco-bbob-1-24', '2009', 'data', 'NEWUOA ') +
                # join_path(data_path, 'gecco-bbob-1-24', '2012', 'data', 'loshchilov_NIPOPaCMA_noise-free-pickle ') +
                join_path(data_path, 'gecco-bbob-1-24', '2009', 'data', 'RANDOMSEARCH ') +
                join_path(data_path, 'gecco-bbob-1-24', '2009', 'data', 'BFGS '))
    print '  subtest finished in ', time.time() - t0, ' seconds'
    t0 = time.time()
    os.system(python + command + '--conv' + 
                join_path(data_path, 'gecco-bbob-1-24', '2009', 'data', 'BFGS'))
    print '  subtest finished in ', time.time() - t0, ' seconds'
    t0 = time.time()
    os.system(python + command + '--conv' +
                join_path(data_path, 'gecco-bbob-1-24', '2013', 'data', 'hutter2013_SMAC.tar.gz') +
                join_path(data_path, 'gecco-bbob-1-24', '2013', 'data', 'auger2013_lmmCMA.tar.gz'))
    print '  subtest finished in ', time.time() - t0, ' seconds'
    t0 = time.time()
    os.system(python + command + ' --omit-single ' +
                join_path(data_path, 'gecco-bbob-1-24', '2009', 'data', 'DE-PSO ') +
                join_path(data_path, 'gecco-bbob-1-24', '2009', 'data', 'VNS '))
    print '  subtest finished in ', time.time() - t0, ' seconds'
    t0 = time.time()
    os.system(python + command + ' --expensive ' +
                join_path(data_path, 'gecco-bbob-1-24', '2009', 'data', 'VNS '))
    print '  subtest finished in ', time.time() - t0, ' seconds'
    print('launching doctest (it might be necessary to close a few pop up windows to finish)')
    t0 = time.time()

    if 1 < 3:
        failure_count = 0
        test_count = 0
        #doctest.testmod(report=True, verbose=True)  # this is quite cool!
        # go through the py files in the bbob_pproc folder
        os.chdir(os.path.dirname(os.path.realpath(__file__)))
        for root, dirnames, filenames in os.walk(os.path.dirname(os.path.realpath(__file__))):
          for filename in fnmatch.filter(filenames, '*.py'):
            current_failure_count, current_test_count = doctest.testfile(os.path.join(root, filename), report=True, verbose=True, module_relative=False)              
            failure_count += current_failure_count
            test_count += current_test_count
    else:
        stdout = sys.stdout
        fn = '_bbob_pproc_doctest_.txt'
        try:
            with open(fn, 'w') as f:
                sys.stdout = f
                doctest.testmod(report=True)
        finally:
            sys.stdout = stdout
        process_doctest_output(fn)
    print 'doctest finished in ', time.time() - t0, ' seconds'
    # print('    more info in file _bbob_pproc_doctest_.txt)')
    print '*** done testing module bbob_pproc ***'
    
#    if (failure_count > 0):
#        raise ValueError('%d of %d tests failed' % (failure_count, test_count))
    
"""     
        sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))
        import bbob_pproc as bb
        print dir(bb)
        #bb=imp.load_module("bbob_pproc",*imp.find_module("bbob_pproc"))
        #pproc=imp.load_module("pproc",*imp.find_module("pproc"))
        #print pproc
        #doctest.testmod(bb.pproc,verbose=True)
        for s in dir(bb):
            if(inspect.ismodule(eval("bb."+s)) and s[:2]!="__"):
                print "bb."+s
                doctest.testmod(eval("bb."+s),verbose=False)                    
        print bb.__all__     
"""     
