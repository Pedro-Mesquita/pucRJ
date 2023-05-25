# -*- coding: utf-8 -*-
"""
Created on Thu May 25 10:25:58 2023

@author: PC-PROF
"""

# ENUNCIADO (não usa recursão)
# Escreva uma função para construir e 
# retornar a tabela de frequência dos 
# caracteres de uma string.
# A tabela deve ser representada como 
# uma lista de listinhas de 2 elementos:
#     primeiro elem da listinha: caractere
#     segundo elem da listinha: quantidade de ocorrencias
# Exemplo:
#     para 'BANANA' é retornada [['B',1],['A',3],['N',2]]
    
# O uso do método count() de string implicará uma 
# versão muito custosa: para todo novo caractere 
# a string será toda percorrida novamente


# OBS:
# Durante o desenvolvimento da função verificou-se 
# a necessidade de buscar um caractere na lista q
# representa a tabela de freq.
# A função busca retorna a posicao da listinha q 
# tem o caractere como primeiro elemento
# ou -1, se o caractere não existir na tabela
def busca(ltab, c):
    ind=0
    while ind < len(ltab):
        if ltab[ind][0]==c:
            return ind
        ind+=1
    return -1

def busca2(ltab, c):
    for ind, itemTab in enumerate(ltab):
        if itemTab[0] ==c:
            return ind
    return -1


def constroi_tab_freq(s):
    
    lTabFreq=[]
    for c in s:
        # é necessário localizar c na tabela, se ele existir
        posNaTabela= busca(lTabFreq, c)
        if posNaTabela == -1:
             lTabFreq.append([c,1])  
        else:
             lTabFreq[posNaTabela][1]+=1
    return lTabFreq

#BLOCO PRINCIPAL:
    
s='ALABAMA MUITO LONGE'

ltf= constroi_tab_freq(s)
print(s)
print('Tabela de Frequencia dos caracteres:')
print(ltf)
    