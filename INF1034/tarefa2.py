# ============= ex1

# import turtle
# t = turtle.Turtle()

# def quadrados(t):
#     for x in range(10):
#         for i in range(4):
#             t.fd(200-20*x)
#             t.lt(90)
        
# quadrados(t)


# ============= ex2

# def soma_e_media(min, max):
#         soma = ((min + max) * (max))/2
#         media = soma/(max)
#         print('O valor da soma é %d, e o valor da média é %.1f'%(soma, media))
                
# soma_e_media(1,100)

# ============= ex3
# def tabuada():
#     for x in range(1, 11):
#         for i in range(1, 11):
#             print(x*i)
#         print("")

# tabuada()

# ============= ex4
n = int(input("Insira um número ímpar: "))
while n % 2 == 0:
    n = int(input("Insira um número ímpar: "))
x = float(input("Insira o valo do ângulo: "))

def calcula_sen(n, x):
    

# turtle.mainloop()