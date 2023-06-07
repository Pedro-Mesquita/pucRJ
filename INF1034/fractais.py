import turtle

t = turtle.Turtle()
t.speed(0)
t.up()
t.goto(-100,-100)
t.down()

def quadrados(t, line):
    for i in range(4):
        t.fd(line)
        t.lt(90)

    line  =  (line - 20)/2
    if line < 20:
        return
    for i in range(4):
        quadrados(t, line)
        t.fd(2*line+20)
        t.lt(90)

# quadrados(t, 300)

# def star(t, size):
#     if size <= 10:
#         return
#     for i in range(5):
#         t.fd(size)
#         star(t, size/2)
#         t.left(216)

def star(t, size, i):
    if i == 3:
        if size <= 10:
            return
    for i in range(5):
        t.fd(size)
        star(t, size/2)
        t.left(216)
    


star(t, 320)



t.getscreen()._root.mainloop() 
