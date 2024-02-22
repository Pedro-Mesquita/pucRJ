import turtle
import eixos_turtle

t=turtle.Turtle()
t.speed(0)
eixos_turtle.plotEixosCartesianos(t,25,15)

# B.1

x=0
while(x<=10):
    y=(x**(1/2))
    eixos_turtle.plotponto(t,x,-y,15)
    x=x+0.2 


# B.2

x = -5
while(x<=5):
    y = 1/(x**2)
    eixos_turtle.plotponto(t,x,-y,15)
    x=x+0.2 


# B.3

x = -5
while(x<=5):
    y = 2**x
    eixos_turtle.plotponto(t,x,-y,15)
    x=x+0.2 


# B.4

x = -5
while(x<=5):
    y = 5-(x**2)
    eixos_turtle.plotponto(t,x,-y,15)
    x=x+0.2 


# B.5

x = -5
while(x<=5):
    y = (x+2)*(x-2)
    eixos_turtle.plotponto(t,x,-y,15)
    x=x+0.2 


t.getscreen()._root.mainloop()