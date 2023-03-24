import math

angulo = float(input("Ângulo: "))
tamanho_rampa = float(input("Tamanho da rampa: "))
altura_rampa = math.sin(math.radians(angulo))*tamanho_rampa

print('O tamanho da rampa é: %.2f' %(altura_rampa))