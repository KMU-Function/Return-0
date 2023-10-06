with open("sub_test.txt", "r") as f:
    cnt = 0
    while True:
        x = f.readline().split()[1:][::-1]
        y = f.readline().split()[1:][::-1]
        z = f.readline().split()[1:][::-1]
        f.readline()
        
        if x==[] or y==[] or z==[]:
            break
            
        x = int(''.join(x), 16)
        y = int(''.join(y), 16)
        z = int(''.join(z), 16)
        
        if x>=y:
            print(cnt)
            if x-y != z:
                print(cnt)
                print(x)
                print(y)
                print(x-y)
                print(z)
                print()
        elif x<y:
            print(cnt, '-------------------------------------')
            if y-x != z:
                print(cnt)
                print(x)
                print(y)
                print(x-y)
                print(z)
                print()
        cnt += 1
print(cnt)