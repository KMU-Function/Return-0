import os

file_path = os.path.join(os.path.dirname(__file__), "test_impvmul.txt")

try:
    with open(file_path, "r") as f:
        cnt = 0
        while True:
            x = f.readline().split()[1:]
            y = f.readline().split()[1:]
            z = f.readline().split()[1:]
            f.readline()
            
            if x == [] or y == [] or z == []:
                break
                
            x = int(''.join(x), 16)
            y = int(''.join(y), 16)
            z = int(''.join(z), 16)
            
            if x * y != z:
                print(cnt)
                print(hex(x))
                print(hex(y))
                print(hex(x * y))
                print(hex(z))
                print()
                cnt += 1

    print("{0:-<30} Success!".format("Improved Multiplication ") if cnt == 0 else f"Failed in {cnt} Test Vectors")

except FileNotFoundError:
    print(f"The file {file_path} does not exist.")
except Exception as e:
    print(f"An error occurred: {e}")
