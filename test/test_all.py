import os

# # #: Delete previous test vector files
# # os.system('rm -rf ../test_add.txt')
# # os.system('rm -rf ../test_sub.txt')

# # #: run subtract test
# # os.system('python3 test/test_add.py') 
# # os.system('python3 test/test_sub.py') 

# #: run subtract test
op = ['add', 'sub', 'mul', 'shr', 'shl', 'div', 'bar', 'exp', 'impvmul']
for o in op:
    os.system(f'python3 test_{o}.py') 


# # #: Delete previous test vector files
# # os.system('rm -rf test_add.txt')
# # os.system('rm -rf test_sub.txt')



