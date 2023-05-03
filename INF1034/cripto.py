def validar_cpf(cpf):
    cpf_modificado = cpf.replace('-', '')
    cpf_modificado = cpf_modificado.replace('.', '')
    lista_cpf = list(cpf_modificado)
    lista_cpf_modificada = null
    i = 0
    x = 0
    acumulador_um = 0
    acumulador_dois = 0
    multiplicador = 10
    multiplicador_um = 11
    primerio_digito_verificador = 0
    
    while i < 9:
        acumulador_um = (multiplicador - i) * int(lista_cpf[i]) + acumulador_um
        i = i + 1
    if acumulador_um % 11 >= 2:
        primerio_digito_verificador = 11 - acumulador_um % 11
        
    
    while x <= 10:
        acumulador_dois = (multiplicador_um - x) * int(lista_cpf[x]) + acumulador_dois
        x = x + 1
    print(lista_cpf)
    print(acumulador_um)
    print(acumulador_dois)
        
    
        

validar_cpf('111.444.777-05')
        
