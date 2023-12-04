import os

file_path = os.path.join(os.path.dirname(__file__), "test_modexp.txt")

try:
    with open(file_path, "r") as f:
        cnt = 0
        while True:
            x = f.readline().split()[1:]
            y = f.readline().split()[1:]
            modulo = f.readline().split()[1:]
            z = f.readline().split()[1:]
            f.readline()
            
            if x==[] or y==[] or z==[] or modulo==[]:
                break
                
            x = int(''.join(x), 16)
            y = int(''.join(y), 16)
            modulo = int(''.join(modulo), 16)
            z = int(''.join(z), 16)
            
            # print(hex(x))
            # print(hex(y))
            # print(hex(modulo))
            # print(hex(pow(x,y,modulo)))
                
            if pow(x,y,modulo) != z:
                print(cnt)
                print(hex(x))
                print(hex(y))
                print(hex(modulo))
                print(hex(pow(x,y,modulo)))
                print(hex(z))
                print()
                cnt += 1
                    
    print("{0:-<30} Success!".format("Modular Exponentiation ") if cnt == 0 else f"Failed in {cnt} Test Vectors")

except FileNotFoundError:
    print(f"The file {file_path} does not exist.")
except Exception as e:
    print(f"An error occurred: {e}")