# Nome completo: Pedro Mesquita Maia
# Matrícula PUC-Rio: 2312664
# Turma: 33P
# Professor: Joisa de Souza Oliveira
# Declaração de autoria: declaro que este documento foi produzido em sua totalidade por mim

# 1-
def simbolos_multiplicados(multiplicador, simb1, simb2):
    return multiplicador*simb1 + multiplicador*simb2


# 2-
def retorna_anos_meses(meses):
    anos = meses//12
    meses_restantes = meses%12 
    return  '%d anos e %d meses'%(anos, meses_restantes)


# 3-
def string_completa(nome, idade_em_meses):
    parte_inicial = simbolos_multiplicados(len(nome), '#', '=')
    parte_meio = '%s tem %s'%(nome, retorna_anos_meses(idade_em_meses))
    parte_final = simbolos_multiplicados(len(nome), '=', '#')
    print('%s %s %s'%(parte_inicial, parte_meio, parte_final))

nome = input('Nome: ')
idade_em_meses = int(input('Idade em meses: '))

string_completa(nome, idade_em_meses)