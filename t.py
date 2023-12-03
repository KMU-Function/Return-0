def f(x):
    x = hex(x)[2:]
    while len(x) % 2 != 0:
        x = "0"+x
    return "{" + ", ".join(["0x"+x[i:i+2] for i in range(0, len(x), 2)][::-1]) + "};" 

p = 11159946659604985032962713247077305296108118876450998063811791141762760783886489901766884101288950728682816259703473272544478890334881585942726975974782309
q = 7875112690178293943328265109850466543713557808905601662948403087236599089166488292928873803308355334654209468712436853498624115524650843303998763150167621

n = p*q

e = 65537

d = pow(e, -1, (p-1)*(q-1))

phi = (p-1)*(q-1)

tmp = 'd5 14 ae d8 0d ef d8 fc 5f 10 91 82 00 9a 67 ea f6 50 2e 8b e3 ad 24 68 c7 d9 59 8c a6 4b 0c 05 37 06 f5 2f a5 a3 0d 50 6f 92 c3 07 cb e2 09 b8 ed 6a 41 6c 62 e3 85 03 e1 bf 75 97 61 4b b5 65'
tmp = int(''.join(tmp.split()), 16)
print(tmp == p)

# print("static word parr[] = ", f(p))
# print("static word qarr[] = ", f(q))
# print("static word narr[] = ", f(n))
# print("static word earr[] = ", f(e))
# print("static word darr[] = ", f(d))
# print("static word narr[] = ", f(n))
# print("static word phiarr[] = ", f(phi))