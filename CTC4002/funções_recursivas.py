# lista 1

def mul_int(int1, int2):
    if int1 == 0 or int2 == 0:
        return 0
    return int1 + (int1 * mul_int(int1, int2-1))

def len_num(inteiro):
    if inteiro < 10:
        return 1
    return 1 + len_num(inteiro//10)

def qtd_3_ou_4(num):
    if num < 3:
        return 0
    if num % 10 == 3 or num % 10 == 4:
        return 1 + qtd_3_ou_4(num//10)
    return qtd_3_ou_4(num//10)

# def print_por_linha(num):
#     if num < 10:
#         print(num)
#         return 
#     print(num // 10)
#     return print_por_linha(num % 10)

def print_por_linha(num):
    if num < 10:
        print(num)
        return 
    print(num % 10)
    return print_por_linha(num // 10)

def fat(num):
    if num == 0:
        return 1
    return num * fat(num-1)

def len_str(s):
    if s == '':
        return 0
    return 1 + len_str(s[1:])

def find_bool(c, s):
    if s == '':
        return False
    if s[0] == c:
        return True
    return find_bool(c, s[1:])

def print_por_linha(s):
    if s == '':
        print('')
    if len(s) == 1:
        print(s)
        return
    print(s[0])
    return print_por_linha(s[1:])

# def print_por_linha_invertido(s):
#     if len(s) == 0:
#         return ' '
#     if len(s) == 1:
#         print(s)
#         return
#     print(s[-1])
#     return print_por_linha_invertido(s[:-1])


def find(c, s):
    if s == '':
        return 0
    if s[0] == c:
        return 1 + find(c, s[1:])
    return find(c, s[1:])





# lista 2
def conta34(num):
    if num < 34:
        return 0
    if num % 100 == 34:
        return 1 + conta34(num//10)
    return conta34(num//10)

def tem_3_ou_4(num):
    if num < 3:
        return False
    if num % 10 == 3 or num % 10 == 4:
        return True
    return tem_3_ou_4(num // 10)  

def algMenores(num1, num2):
    if num1 < 10:
        return num1 < num2
    if num1 % 10 >= num2 % 10:
        return False
    return algMenores(num1 // 10, num2 // 10)

def soma_mult(num1, num2):
    if num1 < 10:
        return num1 * num2
    return ((num1 % 10) * (num2 % 10)) + soma_mult(num1//10, num2//10)

def sem_a(str1, str2):
    if len(str1) == 0:
        return ''
    if str1[0] == 'a' or str2[0] == 'a':
        return ''
    return str1[0]+str2[0] + sem_a(str1[1:], str2[1:])

lista = [10, 'ab', 2.3, [12,9], 5, [[6,'oi'], 'ta'], 21]

def somaLista(lista):
    soma = 0
    for elemento in lista:
        if type(elemento) == list:
            soma += somaLista(elemento)
        if type(elemento) == float or type(elemento) == int:
            soma += elemento
    return soma

def acha(lista, c):
    soma = 0
    for elemento in lista:
        if type(elemento) == list:
            soma += acha(elemento, c)
        if type(elemento) == str:
            for b in elemento:
                if b == c:
                    soma += 1
    return soma