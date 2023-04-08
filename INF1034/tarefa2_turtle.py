import turtle
import random

def eixos(t):
    t.home()
    t.fd(300)
    triangulo_eixos(t)
    t.home()
    t.left(180)
    t.fd(300)
    triangulo_eixos(t)
    t.home()
    t.left(90)
    t.fd(300)
    triangulo_eixos(t)
    t.home()
    t.right(90)
    t.fd(300)
    triangulo_eixos(t)

########################################## FIGURAS GEOMÉTRICAS

def triangulo_eixos(t):
    t.begin_fill()
    t.left(90)
    t.fd(5)
    t.right(120)
    t.fd(10)
    t.right(120)
    t.fd(10)
    t.right(120)
    t.fd(5)
    t.end_fill()

def triangulo(t, x, y, tamanho, color):

    t.up()
    t.home()
    t.setpos(x,y)
    t.down()
    t.color(color)
    t.begin_fill()
    for i in range(3):
        t.fd(tamanho)
        t.left(120)
       
    t.end_fill()
    t.color('black')

def circulo (t, x, y, raio, color):
    t.up()
    t.home()
    t.setpos(x,y)
    t.down()
    t.fd(raio)
    t.left(90)
    t.color(color)
    t.begin_fill()
    t.circle(raio)
    t.end_fill()
    t.color('black')

def pentagono(t, x, y, tamanho, color):
    t.up()
    t.home()
    t.setpos(x,y)
    t.down()
    t.color(color)
    t.begin_fill()
    for i in range(5):
        t.fd(tamanho)
        t.left(360/5)
    t.end_fill()
    t.color('black')
    
def dodecagono(t, x, y, tamanho, color):
    t.up()
    t.home()
    t.setpos(x,y)
    t.down()
    t.color(color)
    t.begin_fill()
    for i in range(12):
        t.fd(tamanho)
        t.left(360/12)
    t.end_fill()
    t.color('black')

def retangulo(t, x, y, altura, lado, color):
    t.up()
    t.home()
    t.setpos(x,y)
    t.down()
    t.color(color)
    t.begin_fill()
    for i in range(2):
        t.left(360/4)
        t.fd(lado)
        t.left(360/4)
        t.fd(altura)
    t.end_fill()
    t.color('black')

def espiral(t, x, y, angulo, largura, color):
    t.up()
    t.home()
    t.setpos(x,y)
    t.down()
    t.color(color)
    for i in range(20):
        t.fd(largura+i)
        t.left(angulo)

def estrela(t, x, y, tamanho, color):
    t.up()
    t.home()
    t.setpos(x,y)
    t.down() 
    t.color(color)
    t.begin_fill()
    for p in range(5):
        t.forward(tamanho)
        t.right(144)
        t.forward(tamanho)
        t.left(144)
        t.right(72)
    t.end_fill()

########################################## BANDEIRAS

def bandeira_austria(t, x, y, largura, altura):
    retangulo(t, x, y, largura, altura, 'red')
    retangulo(t, x, y+(altura/3), largura, altura/3, 'white')

def bandeira_gana(t, x, y, largura_bandeira, altura_bandeira):
    retangulo(t, x, y, largura_bandeira, altura_bandeira/3, 'green')
    retangulo(t, x, y+(altura_bandeira/3), largura_bandeira, altura_bandeira/3, 'yellow')
    retangulo(t, x, y+((altura_bandeira/3)*2), largura_bandeira, altura_bandeira/3, 'red')
    estrela(t, x-(largura_bandeira/2.2), y+(altura_bandeira/3)+(altura_bandeira/5), altura_bandeira/7.5, 'black')

def bandeira_grecia(t, x, y, largura_bandeira, altura_bandeira):
    retangulo(t,x,y, largura_bandeira, altura_bandeira/9, '#0c59a6')
    for i in range(0,9,2):
        retangulo(t,x,y+((altura_bandeira/9)*i), largura_bandeira, altura_bandeira/9, '#0c59a6')
    retangulo(t, x+(altura_bandeira/2)-(largura_bandeira), y+((altura_bandeira/9)*4), altura_bandeira/2, altura_bandeira/2, '#0c59a6')
    retangulo(t, x-largura_bandeira+((largura_bandeira/9)*2.2), y+((altura_bandeira/9)*4), (largura_bandeira/9), (altura_bandeira/9)*5, 'white')
    t.up()
    t.setpos(x, y)
    t.left(180)
    t.fd(largura_bandeira)
    t.right(90)
    t.fd((altura_bandeira/9)*6)
    t.color('white')
    t.down()
    t.begin_fill()
    t.fd(altura_bandeira/9)
    t.right(90)
    t.fd((altura_bandeira/9)*4.5)
    t.right(90)
    t.fd(altura_bandeira/9)
    t.right(90)
    t.fd((altura_bandeira/9)*4.5)
    t.end_fill()
    t.up()
    t.home()


########################################## chamadas
t = turtle.Turtle()
t.speed(0)

cor_dodecagono = input("Digite uma cor para o dodecágono: ")
cor_triangulo = input("Digite uma cor para o triângulo: ")
cor_circulo = input("Digite uma cor para o círculo: ")
cor_pentagono = input("Digite uma cor para o pentágono: ")


eixos(t)

dodecagono(t, random.randint(10, 100), random.randint(10, 100), 20, cor_dodecagono)
triangulo(t, random.randint(-100, -60), random.randint(10, 100), 50, cor_triangulo)
circulo(t, random.randint(-100, -10), random.randint(-100, -10), 20, cor_circulo)
pentagono(t, random.randint(10, 100), random.randint(-100, -10), 10, cor_pentagono)

espiral(t, random.randint(150, 300), random.randint(150, 300), 30, 10, 'purple')
bandeira_austria(t, random.randint(-300, -150), random.randint(150, 300), 200, 150)
bandeira_gana(t, random.randint(-300, -150), random.randint(-300, -150), 200, 150)
bandeira_grecia(t, random.randint(200, 300), random.randint(-350, -300),200, 150)


t.getscreen()._root.mainloop()


