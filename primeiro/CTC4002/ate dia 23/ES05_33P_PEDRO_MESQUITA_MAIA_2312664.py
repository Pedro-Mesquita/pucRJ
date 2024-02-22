import random 

valor_inicial = int(input("Valor inicial: "))
valor_final = int(input("Valor final: "))
lval_intervalo = list(range(valor_inicial, valor_final))
print(lval_intervalo)
valor = int(input("Digite um valor: "))
while valor >= len(lval_intervalo):
    valor = int(input("Digite um valor menor: "))
lq= random.sample(lval_intervalo, valor)
print(lq)
random.shuffle(lval_intervalo)
print(lval_intervalo)