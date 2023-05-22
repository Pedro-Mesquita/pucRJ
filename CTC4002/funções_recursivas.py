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
