import turtle

def ploteixo(t,eixo,tam,larg,cor,esp):
    #Define o tamanho e parâmetros do eixo
    t.width(esp)
    t.color(cor)
    t.up()
    if(eixo == 'x'):
        sentido=0
        posx=-tam/2
        posy=0
        sentidoTraco=90
        deslocxTraco=larg
        deslocyTraco=0
    else:
        sentido=90
        posx=0
        posy=-tam/2
        sentidoTraco=0
        deslocxTraco=0
        deslocyTraco=larg
    desenhaReta(t,sentido,posx,posy,tam)
    desenhaTracoReta(t,sentidoTraco, tam, larg, deslocxTraco, deslocyTraco)
    return

def desenhaReta(t,sentido,posx,posy,tam):
    t.setheading(sentido)
    t.goto(posx,posy)
    t.down()
    t.forward(tam)
    return

def plotEixosCartesianos(t,npontos,larg):
    """Desenha eixo cartesiano com n pontos e dista entre os pontos=larg"""
    #Define o tamanho e parâmetros dos eixos#
    tamanho_reta=2*npontos * larg
    ploteixo(t,'x',tamanho_reta,larg,'gray',1)
    ploteixo(t,'y',tamanho_reta,larg,'gray',1)
    return

def desenhaTracoReta(t,sentidoTraco,tam,larg,deslocx,deslocy):
    t.color('blue')
    t.width(0.5)
    t.setheading(sentidoTraco)
    x=t.xcor()
    y=t.ycor()
    for i in range(tam//larg+1):
        t.up()
        t.goto(x,y)
        t.down()
        t.fd(5)
        t.bk(5)
        x=x-deslocx
        y=y-deslocy
    return

def plotponto(t,x,y,larg):
    t.width(1)
    t.up()
    t.goto(x*larg,y*larg)
    t.down()
    t.dot('red')
    return