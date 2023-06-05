import turtle
t = turtle.Turtle()

def squares(t, line):
    for x in range(5):
        t.fd(line)
        t.left(90)
    squares(line/2)

# squares(100)
def star(t, size):
    if size <= 10:
        return
    for i in range(5):
        t.fd(size)
        star(t, size/2)
        t.left(216)


star(t, 320)



t.getscreen()._root.mainloop() 