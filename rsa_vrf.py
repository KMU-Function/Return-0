import math

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    g, y, x = egcd(b%a,a)
    return (g, x - (b//a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('No modular inverse')
    return x%m

def get_C_array_hex(n):
    n = hex(n)[2:]
    while len(n) % 8 != 0:
        n = "0" + n
    narr = [n[i:i+4] for i in range(0,len(n),4)][::-1]
    for i in range(len(narr)):
        narr[i] = "0x" + narr[i]
    return "{" + ", ".join(narr) + "}"

p = 11159946659604985032962713247077305296108118876450998063811791141762760783886489901766884101288950728682816259703473272544478890334881585942726975974782309
q = 7875112690178293943328265109850466543713557808905601662948403087236599089166488292928873803308355334654209468712436853498624115524650843303998763150167621

e = 65537
phi = (p-1)*(q-1)
d = modinv(e, phi)
n=p*q
print(d)
# print(get_C_array_hex(p))

print("p = ", get_C_array_hex(p))
print("q = ", get_C_array_hex(q))
print("phi = ", get_C_array_hex(phi))
print("e = ", get_C_array_hex(e))
print("d = ", get_C_array_hex(d))
print("n = ", get_C_array_hex(p*q))

m = 123213

c = pow(m, e, n)
pt = pow(c, d, n)

print(m == pt)