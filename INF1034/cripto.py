#ex 1

def validar_cpf(cpf):
    cpf_modificado = cpf.replace('-', '')
    cpf_modificado = cpf_modificado.replace('.', '')
    lista_cpf = list(cpf_modificado)
    lista_top = lista_cpf[0:9]
    acumulador_um = acumulador_dois = primerio_digito_verificador = segundo_digito_verificador = i = x = 0
    multiplicador = 10
    multiplicador_um = 11
    
    while i < 9:
        acumulador_um = (multiplicador - i) * int(lista_cpf[i]) + acumulador_um
        i = i + 1
        
    if acumulador_um % 11 >= 2:
        primerio_digito_verificador = 11 - acumulador_um % 11
        
    lista_top.append(str(primerio_digito_verificador))
    
    while x < 10:
        acumulador_dois = (multiplicador_um - x) * int(lista_top[x]) + acumulador_dois
        x = x + 1
    
    if acumulador_dois % 11 >= 2:
        segundo_digito_verificador = 11 - acumulador_dois % 11
    lista_top.append(str(segundo_digito_verificador))
  
    if str(primerio_digito_verificador) == lista_cpf[9] and str(segundo_digito_verificador) == lista_cpf[10]:
        return True
    else:
        return False
    
print(validar_cpf('111.444.777-05'))
        
#ex 2