# alo mundo com organização padrao

import pygame

width = 800  #Largura Janela
height = 600 #Altura Janela

WHITE = [255, 218, 166]
global result
result = ""

def load():
    global sys_font
    sys_font = pygame.font.Font(pygame.font.get_default_font(), 50)

def check_click(x1,y1,w1,h1,x2,y2):
     return x1 < x2+1 and x2 < x1+w1 and y1 < y2+1 and y2 < y1+h1
    
def draw_screen(screen):
    screen.fill(WHITE)

    #Cria janela de resultado
    pygame.draw.rect(screen, (255,177,69    ), (50, 100, 700, 100))
    t = sys_font.render(result, False, WHITE)
    screen.blit(t, (85,125))
    
    #Cria Botão 1 (contém numero 7)
    pygame.draw.rect(screen, (116, 171, 144), (50, 250, 60, 60))
    t = sys_font.render("7", False, WHITE)
    screen.blit(t, (65,265))
    
    #Cria Botão 2 (contém numero 8)
    pygame.draw.rect(screen, (116, 171, 144), (120, 250, 60, 60))
    t = sys_font.render("8", False, WHITE)
    screen.blit(t, (135,265))
    
    #Cria Botão 3 (contém numero 9)
    pygame.draw.rect(screen, (116, 171, 144), (190, 250, 60, 60))
    t = sys_font.render("9", False, WHITE)
    screen.blit(t, (205,265))

    #Cria Botão 4 (contém numero 4)
    pygame.draw.rect(screen, (116, 171, 144), (50, 320, 60, 60))
    t = sys_font.render("4", False, WHITE)
    screen.blit(t, (65,335))
    
    #Cria Botão 5 (contém numero 5)
    pygame.draw.rect(screen, (116, 171, 144), (120, 320, 60, 60))
    t = sys_font.render("5", False, WHITE)
    screen.blit(t, (135,335))

    #Cria Botão 6 (contém numero 6)
    pygame.draw.rect(screen, (116, 171, 144), (190, 320, 60, 60))
    t = sys_font.render("6", False, WHITE)
    screen.blit(t, (205,335))

    #Cria Botão 7 (contém numero 1)
    pygame.draw.rect(screen, (116, 171, 144), (50, 390, 60, 60))
    t = sys_font.render("1", False, WHITE)
    screen.blit(t, (65,405))
    
    #Cria Botão 8 (contém numero 2)
    pygame.draw.rect(screen, (116, 171, 144), (120, 390, 60, 60))
    t = sys_font.render("2", False, WHITE)
    screen.blit(t, (135,405))

    #Cria Botão 9 (contém numero 3)
    pygame.draw.rect(screen, (116, 171, 144), (190, 390, 60, 60))
    t = sys_font.render("3", False, WHITE)
    screen.blit(t, (205,405))

    #Cria Botão 10 (contém x)
    pygame.draw.rect(screen, (255,177,69), (50, 460, 60, 60))
    t = sys_font.render("C", False, WHITE)
    screen.blit(t, (65,475))

    #Cria Botão 11 (contém numero 3)
    pygame.draw.rect(screen, (116, 171, 144), (120, 460, 60, 60))
    t = sys_font.render("0", False, WHITE)
    screen.blit(t, (135,475))
    
    #Cria Botão 12 (contém numero 3)
    pygame.draw.rect(screen, (255,177,69), (190, 460, 60, 60))
    t = sys_font.render(".", False, WHITE)
    screen.blit(t, (205,475))
    
    

    #Cria Botão 3 (contém operador *)
    pygame.draw.rect(screen, (255,177,69), (260, 250, 60, 60))
    t = sys_font.render("+", False, WHITE)
    screen.blit(t, (275,265))

    #Cria Botão 3 (contém operador +)
    pygame.draw.rect(screen, (255,177,69), (330, 250, 60, 60))
    t = sys_font.render("-", False, WHITE)
    screen.blit(t, (345,265))

    pygame.draw.rect(screen, (255,177,69), (260, 320, 60, 60))
    t = sys_font.render("*", False, WHITE)
    screen.blit(t, (275,335))

    #Cria Botão 3 (contém operador +)
    pygame.draw.rect(screen, (255,177,69), (330, 320, 60, 60))
    t = sys_font.render("/", False, WHITE)
    screen.blit(t, (345,335))

    pygame.draw.rect(screen, (255,177,69), (260, 390, 60, 60))
    t = sys_font.render("//", False, WHITE)
    screen.blit(t, (275,405))

    #Cria Botão 3 (contém operador +)
    pygame.draw.rect(screen, (255,177,69), (330, 390, 60, 60))
    t = sys_font.render("%", False, WHITE)
    screen.blit(t, (345,405))

    pygame.draw.rect(screen, (255,177,69), (260, 460, 60, 60))
    t = sys_font.render("**", False, WHITE)
    screen.blit(t, (275,475))

    #Cria Botão 3 (contém operador +)
    pygame.draw.rect(screen, (116, 171, 144), (330, 460, 60, 60))
    t = sys_font.render("=", False, WHITE)
    screen.blit(t, (345,475))



    pass

def mouse_click_down(px_mouse, py_mouse, mouse_buttons):
    global result
    if mouse_buttons[0]:
        if check_click(50, 250, 60, 60, px_mouse, py_mouse):
            result = result + "7"
        if check_click(120, 250, 60, 60, px_mouse, py_mouse):
            result = result + "8"
        if check_click(190, 250, 60, 60, px_mouse, py_mouse):
            result = result + "9"
        if check_click(50, 320, 60, 60, px_mouse, py_mouse):
            result = result + "4"
        if check_click(120, 320, 60, 60, px_mouse, py_mouse):
            result = result + "5"
        if check_click(190, 320, 60, 60, px_mouse, py_mouse):
            result = result + "6"
        if check_click(50, 390, 60, 60, px_mouse, py_mouse):
            result = result + "1"
        if check_click(120, 390, 60, 60, px_mouse, py_mouse):
            result = result + "2"
        if check_click(190, 390, 60, 60, px_mouse, py_mouse):
            result = result + "3"
        if check_click(120, 460, 60, 60, px_mouse, py_mouse):
            result = result + "0"
        if check_click(260, 320, 60, 60, px_mouse, py_mouse):
            result = result + "*"  
        if check_click(260, 250, 60, 60, px_mouse, py_mouse):
            result = result + "+"   
        if check_click(330, 250, 60, 60, px_mouse, py_mouse):
            result = result + "-"   
        if check_click(330, 320, 60, 60, px_mouse, py_mouse):
            result = result + "/"   
        if check_click(260, 390, 60, 60, px_mouse, py_mouse):
            result = result + "//"   
        if check_click(330, 390, 60, 60, px_mouse, py_mouse):
            result = result + "%"   
        if check_click(260, 460, 60, 60, px_mouse, py_mouse):
            result = result + "**"   

        if check_click(190, 460, 60, 60, px_mouse, py_mouse):
            result = result + "."   
        if check_click(50, 460, 60, 60, px_mouse, py_mouse):
            result = result[0:len(result)-1]   


        if check_click(330, 460, 60, 60, px_mouse, py_mouse):
            result = str(eval(result))
               
def main_loop(screen):
    running = True
    while running:
        for e in pygame.event.get(): 
            if e.type == pygame.QUIT:
                running = False
                break
            elif e.type == pygame.MOUSEBUTTONDOWN: #detecta o inicio do clique do mouse
                    mouse_buttons = pygame.mouse.get_pressed()
                    px_mouse, py_mouse = pygame.mouse.get_pos()
                    mouse_click_down(px_mouse, py_mouse, mouse_buttons)
                    
        # Desenha objetos na tela 
        draw_screen(screen)
        # Pygame atualiza o seu estado
        pygame.display.update() 


pygame.init()
screen = pygame.display.set_mode((width, height))
load()
main_loop(screen)
pygame.quit()
