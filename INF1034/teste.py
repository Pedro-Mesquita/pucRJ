import turtle
import random

joaquim = turtle.Turtle()
joaquim.color("orange")
joaquim.shape("turtle")
joaquim.speed(2)

maria = turtle.Turtle()
maria.color("blue")
maria.shape("turtle")
maria.speed(2)

joaquim.up()
joaquim.setposition(-200,100)
joaquim.down()
maria.up()
maria.setposition(-200,0)
maria.down()
joaquim.up()
joaquim.down()

for x in range(10):
    dado1 = random.randint(1,6)*10
    dado2 = random.randint(1,6)*10
    joaquim.fd(dado1)
    maria.fd(dado2)
