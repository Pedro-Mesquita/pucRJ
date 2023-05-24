import turtle
import eixos_turtle
import random


texto = 'Este samba Vai pra Dorival Caymmi João Gilberto e Caetano Veloso Vamo lá  O Rio de Janeiro continua lindo O Rio de Janeiro continua sendo O Rio de Janeiro fevereiro e março  Alô, alô Realengo aquele abraço Alô torcida do Flamengo aquele abraço Alô alô Realengo aquele abraço Alô torcida do Flamengo aquele abraço  Chacrinha continua balançando a pança E buzinando a moça e comandando a massa E continua dando as ordens no terreiro  Alô alô seu Chacrinha velho guerreiro Alô alô Terezinha Rio de Janeiro Alô alô seu Chacrinha velho palhaço Alô alô Terezinha aquele abraço  Alô moça da favela aquele abraço Todo mundo da Portela aquele abraço Todo mês de fevereiro aquele passo Alô Banda de Ipanema aquele abraço  Meu caminho pelo mundo eu mesmo traço A Bahia já me deu régua e compasso Quem sabe de mim sou eu Aquele abraço Pra você que me esqueceu Aquele abraço  Alô Rio de Janeiro aquele abraço Todo o povo brasileiro aquele abraço  O Rio de Janeiro continua lindo O Rio de Janeiro continua sendo O Rio de Janeiro, fevereiro e março  Alô alô Realengo Alô torcida do Flamengo aquele abraço Alô alô Realengo aquele abraço Alô torcida do Flamengo aquele abraço  Chacrinha continua balançando a pança E buzinando a moça e comandando a massa E continua dando as ordens no terreiro  Alô alô seu Chacrinha velho guerreiro Alô alô Terezinha Alô alô seu Chacrinha Alô alô Terezinha aquele abraço  Alô moça da favela aquele abraço Todo mundo da Portela aquele abraço Todo mês de fevereiro aquele passo Alô Banda de Ipanema aquele abraço  Meu caminho pelo mundo eu mesmo traço A Bahia já me deu graças a Deus régua e compasso Quem sabe de mim sou eu é claro aquele abraço Pra você que me esqueceu Aquele abraço  Alô Rio de Janeiro todo mundo aquele abraço Todo o povo brasileiro aquele abraço Todo mês de fevereiro aquele abraço Alô moça da favela aquele abraço Todo mundo da Portela e do Salgueiro e todo o Rio de Janeiro aquele abraço E todo mês de fevereiro e todo povo brasileiro aquele abraço  Alô minha nega-samba aquele abraço E alô alô da pipoca aquele abraço E todo Rio de Janeiro aquele abraço E todo povo brasileiro todo povo brasileiro Todo povo brasileiro, todo povo brasileiro Todo povo brasileiro (aquele abraço!)  (Aquele abraço!) (Aquele abraço!) (Aquele abraço!) (Aquele abraço!)'
texto = texto.upper()
texto = texto.replace(',', '').replace('(', '').replace(')', '').replace('!', '').split()
lista = texto
lista_filtrada = []
cores = ['red','green','blue','yellow','magenta','cyan','pink','gray','black','brown']
for c in lista:
    if len(c) > 3:
        lista_filtrada.append(c)
        

lista_filtrada_sem_repeticao = list(dict.fromkeys(lista_filtrada))
    
lista_palavras_e_incidencia = []
    
for palavra in lista_filtrada_sem_repeticao:
    lista_palavras_e_incidencia.append([palavra, lista_filtrada.count(palavra)])
    
lista_palavras_e_incidencia =  sorted(lista_palavras_e_incidencia, key=lambda x: x[1], reverse=True)

t = turtle.Turtle()
eixos_turtle.plotEixosCartesianos(t,25,15)

def pos_tartaruga(t, x, y):
    t.fd(x)
    t.left(y)
    

t.setpos(0,0)
t.right(20)
for i in range(19):
    pos_tartaruga(t, 60*(i*0.5), 40)
    t.color(random.choice(cores))
    t.write(lista_palavras_e_incidencia[i][0], move=True,align='right',font=('Arial',15,'bold'))
    t.color('black')

