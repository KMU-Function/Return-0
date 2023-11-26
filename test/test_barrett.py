# with open("test/test_sub.txt", "r") as f:
with open("test_barrett.txt", "r") as f:
    cnt = 0
    while True:
        x = f.readline().split()[1:]
        y = f.readline().split()[1:]
        z = f.readline().split()[1:]
        f.readline()
        
        if x==[] or y==[] or z==[]:
            break
            
        x = int(''.join(x), 16)
        y = int(''.join(y), 16)
        z = int(''.join(z), 16)
        
        if x % y != z:
            print(cnt)
            print(hex(x))
            print(hex(y))
            print(hex(x % y))
            print(hex(z))
            cnt += 1
                
print("{0:-<20} Success!".format("Barrett Reduction ") if cnt == 0 else f"Failed in {cnt} Test Vectors")