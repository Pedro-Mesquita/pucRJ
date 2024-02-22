# -*- coding: utf-8 -*-
"""
Created on Tue Jun 13 09:17:02 2023

@author: JOISA
"""

# ExA) Escreva uma função que receba um curso e um nome 
# de arquivo TEXTO com os dados dos
# alunos, um aluno por linha, e exiba nome e CR de todos 
# os alunos do curso recebido. Os dados
# dos alunos em uma linha estão separados por , e são: 
# o nome completo do aluno, o curso do aluno e o CR do aluno.

def exibeDeUmCurso(nomeArq, curso):
    nomeArq = nomeArq +'.txt'
    arq=  open(nomeArq,'r')
    for linha in arq:
        lsDoAl= linha.strip().split(',')
        if lsDoAl[1]==curso:
            print('Nome: %s - CR:%s'%(lsDoAl[0], lsDoAl[2]))
    arq.close()

# ExB) Escreva uma função que receba um valor X e um nome 
# de arquivo com os dados dos alunos, um aluno por linha, e 
# escreva em um arquivo de nome acimalimite.txt nome, curso e
# CR de todos os alunos com CR > X. Os dados dos alunos em 
# uma linha estão separados por , e são: o nome completo do aluno, 
# o curso do aluno e o CR do aluno.

def criaAcimaDolimite(x, nomeArq):
    nomeArq = nomeArq +'.txt'
    arqEnt=  open(nomeArq,'r')
    arqSai= open('acimalimite.txt','w')
    for linha in arqEnt:
        lsDoAl= linha.strip().split(',')
        if float(lsDoAl[2])>x:
            arqSai.write(linha)
    arqEnt.close()
    arqSai.close()
        

# Escreva um programa completo para testar as funções A e B

# ExC) Considerando o mesmo arquivo dos itens A e B, escreva 
# uma função que receba o nome do arquivo, leia os dados dos 
# alunos e calcule o número de alunos por curso, retornando a
# resposta ao final. A resposta deve ser uma lista 
# de listinhas com [curso,qtdAlunos]


def buscaNaLTabFreq (lTabFreq, curso):
    #listinha lCurQtd é um item da tab de freq com [curso,qtdDoCurso]
    for ind, lCurQtd in enumerate(lTabFreq):
        if lCurQtd[0]== curso:
            return ind  # ACHOU: retornou a posicao da lista TAB de FREQ
    # testou todos e nao achou
    return -1  # poderia retornar None
    
    
def criaTabFreq(nomeArq):
    lTabFreq=[]
    nomeArq = nomeArq +'.txt'
    arqEnt=  open(nomeArq,'r')
    for linha in arqEnt:
        lsDoAl = linha.strip().split(',')
        curso = lsDoAl[1]
        posDoCurso= buscaNaLTabFreq(lTabFreq,curso)
        if posDoCurso != -1:
            lTabFreq[posDoCurso][1]+=1
        else:
            lTabFreq.append( [curso,1] )
    arqEnt.close()
    return lTabFreq
        

# ExD) Considerando o mesmo arquivo dos itens A e B, escreva 
# uma função que receba o nome do arquivo, leia os dados dos 
# alunos e construa e retorne uma lista em que cada elemento 
# é uma listinha com [curso, lista com nomes dos alunos do curso]

# ExE) Considerando o mesmo arquivo dos itens A e B, escreva 
# uma função que receba o nome do arquivo, leia os dados dos 
# alunos e crie um novo arquivo, denominado cursosealunos.txt 
# com a seguinte estrutura:
# curso 1, quantidade de alunos do curso 1
# aluno do curso 1, CR
# aluno do curso 1, CR
# ...
# curso 2, quantidade de alunos do curso 2
# aluno do curso 2, CR

# ExF)  Considerando o mesmo arquivo dos itens A e B, e um 
# segundo arquivo de nome trancados.txt com os nomes dos 
# alunos que trancaram seus cursos, escreva uma função 
# que receba os nomes dos dois arquivos e crie um terceiro, 
# nomeado como atualizados.txt, somente com os dados dos 
# alunos que não trancaram seus cursos.






#Programa Principal
# exibeDeUmCurso('alunosecursos','ENGENHARIA AMBIENTAL')
# criaAcimaDolimite(7.5,'alunosecursos')
lTabFreqDosCursos= criaTabFreq('alunosecursos')
print(lTabFreqDosCursos)
