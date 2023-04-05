# 1) Desenhar o plano cartesiano (eixos x e y) (opcional, não incluso na nota: com a seta ao final de cada um e numerados);
# 2) Desenhar uma forma geométrica, pelo menos, em cada quadrante (usar repetição e fazer pelo menos duas diferentes da sala de aula, com cores distintas para cada);
# 3) Desenhar uma espiral (sentido horário ou anti-horario) em algum dos quadrantes;
# 4) Desenhar uma bandeira fácil (Marrocos, ou uma com 2 a 3 formas geométricas, não necessariamente diferentes);
# 5) Desenhar uma bandeira média (Turquia, ou uma com 3 a 4 formas geométricas, não necessariamente diferentes);

# 6) Desenhar uma bandeira difícil (África do Sul, ou uma com mais de 4 formas geométricas, não necessariamente diferentes);
# DESAFIOS (opcional, não incluso na nota)
# - Deve ser usado textinput para alguma funcionalidade (mudança de cor, escolher forma geométrica, etc.);
# - Implementar o código usando uma função para cada elemento diferente (bandeira, plano, forma geométrica, etc.)

import turtle
import math
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

def retangulo(t, x, y, lado, altura, color):
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

def espiral(t, x, y, angulo, color):
    t.up()
    t.home()
    t.setpos(x,y)
    t.down()
    t.color(color)
    for i in range(20):
        t.fd(4+i)
        t.left(angulo)


    
t = turtle.Turtle()
t.speed(0)
eixos(t)
espiral(t, random.randint(10,300), random.randint(10,300), 30, 'purple' )
dodecagono(t, random.randint(10,300), random.randint(10,300), 20, 'black' )
triangulo(t, random.randint(-300 ,-10), random.randint(10,300), 70, 'yellow')
circulo(t, random.randint(-300 ,-10), random.randint(-300,-10), 70, 'lightgreen')
pentagono(t, random.randint(10 , 300), random.randint(-300,-10), 40, 'skyblue')

