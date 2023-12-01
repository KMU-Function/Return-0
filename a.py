# import os

# # Change to the 'build' directory
# #os.chdir('build')

# # Run the test executable
# os.system('make')
# os.system('./build/test_executable')

# # Move back to the 'test' directory
# os.chdir('test')

# # Run the Python test script
# os.system('python3 test_all.py')
import os
import subprocess

try:
    print("Current working directory:", os.getcwd())

    # Run 'make' command
    make_result = subprocess.run('make', shell=True, check=True)
    print("Make command executed successfully")

    # Run the test executable
    subprocess.run('./build/test_executable', shell=True, check=True)
    print("Test executable executed successfully\n")

    # Run the Python scripts 'test_add.py' and 'test_sub.py' using relative paths
    script_paths = ['test/test_add.py', 'test/test_sub.py', 'test/test_mul.py', 'test/test_shr.py', 'test/test_shl.py', 
                    'test/test_sqr.py', 'test/test_div.py', 'test/test_bar.py', 'test/test_exp.py' ]

    for script_path in script_paths:
        subprocess.run(f'python3 {script_path}', shell=True, check=True)
        print(f"Python script '{script_path}' executed successfully\n")

except subprocess.CalledProcessError as e:
    print(f"Error: {e}")
except Exception as e:
    print(f"An unexpected error occurred: {e}")
