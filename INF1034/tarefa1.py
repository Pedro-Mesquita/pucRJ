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

#definções básicas
t = turtle.Turtle()
t.color("black")
t.screen.setup(800, 800)
t.screen.screensize(800, 800)
t.speed(100000000)

def triangulo(t):
    t.begin_fill()
    t.right(90)
    t.fd(5)
    t.left(120)
    t.fd(10)
    t.left(120)
    t.fd(10)
    t.left(120)
    t.fd(5)
    t.end_fill()
    
def posicao(t, x, y):
    t.up()
    t.setposition(x,y)
    t.down()

for x in range(9):
    if x <= 9:
        t.write(x)
    t.fd(20)

triangulo(t)

t.home()
t.left(180)
for x in range(9):
    if x <= 9:
        t.write(x*-1)
    t.fd(20)
 
triangulo(t)
   
t.home()  
t.left(90)      
for x in range(9):
    if x == 0:
        print()
    elif x<=9:
        t.write(x, align="right")
    t.fd(20)

triangulo(t)

t.home()
t.right(90)
for x in range(9):
    if x == 0:
        print()
    elif x <= 9:
        t.write(x*-1, align="right")
    t.fd(15)
triangulo(t)

posicao(t, 50, 50)
t.color("red")
t.begin_fill()
for x in range(12):
    t.right(360/12)
    t.fd(30)    
t.end_fill()
posicao(t,-50,50)
t.color("yellow")
t.begin_fill()
for x in range(3):
    t.fd(110)
    t.right(120)
t.end_fill()

posicao(t,-100,-50)

t.color("green")
t.begin_fill()
for i in range(360):
    t.left(360/360)
    t.fd(0.8)
t.end_fill()
    
posicao(t, 50,-120)

t.color("blue")
t.begin_fill()
for x in range(5):
    t.right(360/5)
    t.fd(50)    
t.end_fill()

posicao(t,200,200)

t.color("black")

for x in range(50):
    t.fd(x)
    t.left(30)
t.up()
t.home()
posicao(t,-200,200)
t.color("red")

def bandeira_litras(t,cor_um, cor_dois, cor_tres):
    t.color(cor_um)
    t.begin_fill()
    t.left(180)
    t.fd(100/3)
    t.right(90)
    t.fd(150)
    t.right(90)
    t.fd(100/3)
    t.end_fill()
    
    t.left(180)
    t.fd(100/3)
    t.color(cor_dois)
    t.begin_fill()
    t.left(90)
    t.fd(150)
    t.right(90)
    t.fd(100/3)
    t.right(90)
    t.fd(150)
    t.end_fill()
    
    t.color(cor_tres)
    t.begin_fill()
    t.left(90)
    t.fd(100/3)
    t.left(90)
    t.fd(150)
    t.left(90)
    t.fd(100/3)
    t.end_fill()

t.up()
t.home()
t.down()
t.left(90)
posicao(t, -200, 200)
bandeira_litras(t, "red", "white", "red")

t.up()
t.home()
t.down()
posicao(t,-200,-200)
t.left(90)
bandeira_litras(t, "green", "yellow", "red")
t.left(90)
t.fd(150/1.8)
t.right(105)

t.begin_fill()
t.down()
for i in range(5):
    t.fd(35)
    t.right(144)

t.end_fill()

t.getscreen()._root.mainloop()