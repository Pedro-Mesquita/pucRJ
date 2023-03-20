import random
import string
#1
idade = 21
anoCorrente = 2023
anoFuturo = anoCorrente + 2
print(anoFuturo-anoCorrente + idade)
print("====================")
#2A
primeiroNome = "Pedro"
ultimoNome = "Maia"
nomeInvertido = ultimoNome+", "+primeiroNome
print(nomeInvertido)
print("====================")
#2B
print(primeiroNome[0])
#2C
print(primeiroNome[-1])
#2D
m = len(primeiroNome)//2
n = len(ultimoNome)*2
novaString = m*"*"+n*"@"
print(novaString)
print("====================")
#3
data_de_nascimento = "17/01/2002"

lala_ano_de_nascimento = int(data_de_nascimento[6:])-2
print(lala_ano_de_nascimento)
print("====================")
#4
#help(random.choice)  Escolhe um elemento aleatório de uma sequência não vazia.
nomeCompleto = "pedro mesquita maia"
print(random.choice(nomeCompleto))
alfabeto = string.ascii_lowercase
print(random.choice(alfabeto))