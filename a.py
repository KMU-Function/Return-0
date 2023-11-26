import os

# Change to the 'build' directory
#os.chdir('build')

# Run the test executable
os.system('make')
os.system('./build/test_executable')

# Move back to the 'test' directory
os.chdir('test')

# Run the Python test script
os.system('python3 test_all.py')
