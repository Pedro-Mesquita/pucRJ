p = int(input("Primo um: "))
q = int(input("Primo dois: "))
n = p*q
t = (p-1)*(q-1)

def mdcEstendido(a, b):
    #Caso Base
    if a == 0 :
        return b,0,1
             
    mdc,x1,y1 = mdcEstendido(b%a, a)
     
    x = y1 - (b//a) * x1
    y = x1
     
    return mdc,x,y

def primos_entre_si(totiente):
    primos = []
    for i in range(2, totiente):
        if mdcEstendido(totiente, i)[0] == 1:
            if i > 1 and i < totiente:
                primos.append(i)
    return primos

e = primos_entre_si(t)
print(e)
chave_publica = e[10], n
d = 1

while True:
    if (d * chave_publica[0])%t == 1:
        break
    d+=1

chave_privada = d, n

print(chave_publica)
print(chave_privada)

def encriptador(m):
    return pow(m, chave_publica[0])%chave_publica[1]

encriptado = encriptador(2)
print(encriptado)

def descriptador(m):
    return pow(m, chave_privada[0])%chave_privada[1]

desencriptado = descriptador(encriptado)
print(desencriptado)
