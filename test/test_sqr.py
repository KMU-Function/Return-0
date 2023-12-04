import os

file_path = os.path.join(os.path.dirname(__file__), "test_sqr.txt")

try:
    with open(file_path, "r") as f:
        cnt = 0
        while True:
            x = f.readline().split()[1:]
            y = f.readline().split()[1:]
            f.readline()
            
            if x==[] or y==[]:
                break
                
            x = int(''.join(x), 16)
            y = int(''.join(y), 16)
            
            if x ** 2 != y:
                print(cnt)
                print(hex(x))
                print(hex(x ** 2))
                print(hex(y))
                print()
                cnt += 1
                    
    print("{0:-<30} Success!".format("Sqaure ") if cnt == 0 else f"Failed in {cnt} Test Vectors")

except FileNotFoundError:
    print(f"The file {file_path} does not exist.")
except Exception as e:
    print(f"An error occurred: {e}")