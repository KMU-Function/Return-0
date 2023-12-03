def f(x):
    x = hex(x)[2:]
    while len(x) % 8 != 0:
        x = "0"+x
    return "{" + ", ".join(["0x"+x[i:i+8] for i in range(0, len(x), 8)][::-1]) + "};" 

p = 11159946659604985032962713247077305296108118876450998063811791141762760783886489901766884101288950728682816259703473272544478890334881585942726975974782309
q = 7875112690178293943328265109850466543713557808905601662948403087236599089166488292928873803308355334654209468712436853498624115524650843303998763150167621

n = p*q

e = 65537

d = pow(e, -1, (p-1)*(q-1))

phi = (p-1)*(q-1)

print("static word parr[] = ", f(p))
print("static word qarr[] = ", f(q))
print("static word narr[] = ", f(n))
print("static word earr[] = ", f(e))
print("static word darr[] = ", f(d))
print("static word narr[] = ", f(n))
print("static word phiarr[] = ", f(phi))