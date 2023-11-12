import os

#: Delete previous test vector files
os.system('rm -rf test/test_sub.txt')

#: run subtract test
os.system('../Return-0/main >> test/test_sub.txt')
os.system('python3 test/test_sub.py')


