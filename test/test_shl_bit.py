# with open("test/test_sub.txt", "r") as f:
with open("test_shl_bit.txt", "r") as f:
    cnt = 0
    while True:
        x = f.readline().split()[1:][::-1]
        y = f.readline().split()[1:][::-1]
        z = f.readline().split()[1:][::-1]
        f.readline()
        
        if x==[] or y==[] or z==[]:
            break
        
        intx = int(''.join(x), 16)
        y = int(y[0])
        intz = int(''.join(z), 16)
        
        
        if intx << y != intz:
            print(cnt)
            print(x)
            print(f'{y} bit')
            print(hex(intx << y))
            print(z)
            print()
            cnt += 1
                
print("{0:-<20} Success!".format("shr bit ") if cnt == 0 else f"Failed in {cnt} Test Vectors")