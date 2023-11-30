def barrett_red(A , N , T, w = 32):
    Q_hat = (( A >> (w *(n -1) )) * T ) >> (w *( n +1) )
    print(hex(Q_hat))
    R = A - N* Q_hat
    print(hex(N*Q_hat))
    print(hex(R))
    while R >= N:
        R = R - N
        print(hex(R))
    return R


w2= '00000001 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000'
x= '0617e1c3 486d4a54 36c0c94a 1b4d3235 6a7ea6fd 21b10376 292c276b 016d5dc9 551750c3 2515b4ae 32989910 2dcc6d96'
y= '2d40b8de 0716c420 7feb6c19 008066ce 7cee0fd6 2183f3a6'
t= '00000005 a83817ae cfcfde67 58707202 0376d8d6 18d0ebb1 495a4c98'
r = 'ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff dddddaf3 8c45a754 92976db9 3a3fbfc2 2f638bb6 1cc0b492'


'''
tmp_x = 0617e1c3 486d4a54 36c0c94a 1b4d3235 6a7ea6fd 21b10376 292c276b
qhat = 22786ac2 42d844b9 440a837e 6db044ab cdab05fa 7d5f0a1c bf524a7f 247a772a 79fe89a6 d3200fbb 7e4b4e06 860fad1f ec892b88
qhat = 22786ac2 42d844b9 440a837e 6db044ab cdab05fa 7d5f0a1c
tmp_r = 0617e1c3 486d4a54 36c0c94a 1b4d3235 6a7ea6fd 21b10376 070a025e 8db3051d e7aebe7c 5f557470 61fc24c6 4a8d2228
r = ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff dddddaf3 8c45a754 92976db9 3a3fbfc2 2f638bb6 1cc0b492

'''
n = len(y.split(" "))
w2 = int(''.join(w2.split(" ")), 16)
x = int(''.join(x.split(" ")), 16)
y = int(''.join(y.split(" ")), 16)
t = int(''.join(t.split(" ")), 16)
r = int(''.join(r.split(" ")), 16)

R =barrett_red(x, y, t)


print(hex(r))