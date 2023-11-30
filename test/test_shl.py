# with open("test/test_sub.txt", "r") as f:
with open("test_shl.txt", "r") as f:
    cnt = 0
    while True:
        x = f.readline().split()[1:]
        y = f.readline().split()[1:]
        z = f.readline().split()[1:]
        f.readline()
        
        if x==[] or y==[] or z==[]:
            break
        
        x = int(''.join(x), 16)
        y = int(y[0])
        z = int(''.join(z), 16)
        
        
        if x << y != z:
            print(cnt)
            print(x)
            print(f'{y} bit')
            print(hex(x << y))
            print(z)
            print()
            cnt += 1
                
print("{0:-<20} Success!".format("shl bit ") if cnt == 0 else f"Failed in {cnt} Test Vectors")