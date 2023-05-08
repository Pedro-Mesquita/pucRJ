# #ex 1

# def validar_cpf(cpf):
#     cpf_modificado = cpf.replace('-', '')
#     cpf_modificado = cpf_modificado.replace('.', '')
#     lista_cpf = list(cpf_modificado)
#     lista_top = lista_cpf[0:9]
#     acumulador_um = acumulador_dois = primerio_digito_verificador = segundo_digito_verificador = i = x = 0
#     multiplicador = 10
#     multiplicador_um = 11
    
#     while i < 9:
#         acumulador_um = (multiplicador - i) * int(lista_cpf[i]) + acumulador_um
#         i = i + 1
        
#     if acumulador_um % 11 >= 2:
#         primerio_digito_verificador = 11 - acumulador_um % 11
        
#     lista_top.append(str(primerio_digito_verificador))
    
#     while x < 10:
#         acumulador_dois = (multiplicador_um - x) * int(lista_top[x]) + acumulador_dois
#         x = x + 1
    
#     if acumulador_dois % 11 >= 2:
#         segundo_digito_verificador = 11 - acumulador_dois % 11
#     lista_top.append(str(segundo_digito_verificador))
  
#     if str(primerio_digito_verificador) == lista_cpf[9] and str(segundo_digito_verificador) == lista_cpf[10]:
#         return True
#     else:
#         return False
    
# print(validar_cpf('153.491.037-92'))
        
#ex 2
# import string

# senha = input("Digite uma senha para ser testada: ")

# def verificaSenha(senha):
#     tem_letra_maiuscula = 0
#     tem_letra_minuscula = 0
#     tem_numero = 0
#     numeros = '0123456789'
#     if len(senha) < 8:
#         return False
#     else:
#         for x in range(len(senha)):
#             if senha[x] in string.ascii_uppercase:
#                 tem_letra_maiuscula += 1
#             if senha[x] in string.ascii_lowercase:
#                 tem_letra_minuscula += 1
#             if senha[x] in numeros:
#                 tem_numero += 1
#         if tem_letra_maiuscula != 0 and tem_letra_minuscula != 0 and tem_numero != 0:
#             return True
#         else:
#             return False

# print(verificaSenha(senha))

#ex 3
import string

numeros = '0123456789'
caracteres = numeros+""+string.ascii_lowercase

def criptografa(senha):
    print(senha)
    senha_cripto = ''
    senha_descripto = ''
    for x in range(len(senha)):
        i = caracteres.find(senha[x])
        senha_cripto = senha_cripto+''+caracteres[i+3]
    print(senha_cripto)
    for k in range(len(senha)):
        i = caracteres.find(senha_cripto[k])
        senha_descripto = senha_descripto+''+caracteres[i-3]
    print(senha_descripto)

senha = input("Digite uma senha para ser testada: ")
criptografa(senha)