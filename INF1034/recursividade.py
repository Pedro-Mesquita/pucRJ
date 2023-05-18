def potencia(x, y):
    if y == 0:
        return 1
    return x * potencia(x, y-1)

print(potencia(3,3))
        
def fat(n):
    if n == 0:
        return 1
    return n * fat(n - 1)

print(fat(5))

def fibo(x):
    if x == 1:
        return 1
    if x == 0:
        return 0
    return fibo(x-1) + fibo(x-2)

print(fibo(6))

def somaAlg(numero):
    if numero < 10:
        return numero
    return numero % 10 + somaAlg(numero // 10)

def mostra_num(num):
    if num < 10:
        print(num)
        return num
    numAnt = num//10
    u = num % 10
    mostra_num(numAnt)
    print(u)
    
mostra_num(230)

def mostra_num(num):
    if num < 10:
        return 1
    return 1 + mostra_num(num // 10)

print(mostra_num(2234))

def pares(num):
    if num < 10:
        if num % 2 == 0:
            return 1
        return 0
    if num % 2 == 0:
        return 1 + pares(num // 10)
    return pares(num // 10)


print(pares(123456821))

def bomDia(n):
    if n == 0:
        return
    print("Bom dia")
    return bomDia(n - 1)

bomDia(10)