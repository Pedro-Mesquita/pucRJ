# def potencia(x, y):
#     if y == 0:
#         return 1
#     return x * potencia(x, y-1)

# print(potencia(3,3))
        
# def fat(n):
#     if n == 0:
#         return 1
#     return n * fat(n - 1)

# print(fat(5))

def fibo(x):
    if x == 1:
        return 1
    if x == 0:
        return 0
    return fibo(x-1) + fibo(x-2)

print(fibo(5))