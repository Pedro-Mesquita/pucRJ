# alo mundo com organização padrao

import pygame

width = 440  #Largura Janela
height = 500 #Altura Janela

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
    pygame.draw.rect(screen, (255,177,69), (50, 50, 340, 100))
    t = sys_font.render(result, False, WHITE)
    screen.blit(t, (85,85))
    
    #Cria Botão 1 (contém numero 7)
    pygame.draw.rect(screen, (116, 171, 144), (50, 200, 60, 60))
    t = sys_font.render("7", False, WHITE)
    screen.blit(t, (65,215))
    
    #Cria Botão 2 (contém numero 8)
    pygame.draw.rect(screen, (116, 171, 144), (120, 200, 60, 60))
    t = sys_font.render("8", False, WHITE)
    screen.blit(t, (135,215))
    
    #Cria Botão 3 (contém numero 9)
    pygame.draw.rect(screen, (116, 171, 144), (190, 200, 60, 60))
    t = sys_font.render("9", False, WHITE)
    screen.blit(t, (205,215))

    #Cria Botão 4 (contém numero 4)
    pygame.draw.rect(screen, (116, 171, 144), (50, 270, 60, 60))
    t = sys_font.render("4", False, WHITE)
    screen.blit(t, (65,285))
    
    #Cria Botão 5 (contém numero 5)
    pygame.draw.rect(screen, (116, 171, 144), (120, 270, 60, 60))
    t = sys_font.render("5", False, WHITE)
    screen.blit(t, (135,285))

    #Cria Botão 6 (contém numero 6)
    pygame.draw.rect(screen, (116, 171, 144), (190, 270, 60, 60))
    t = sys_font.render("6", False, WHITE)
    screen.blit(t, (205,285))

    #Cria Botão 7 (contém numero 1)
    pygame.draw.rect(screen, (116, 171, 144), (50, 340, 60, 60))
    t = sys_font.render("1", False, WHITE)
    screen.blit(t, (65,355))
    
    #Cria Botão 8 (contém numero 2)
    pygame.draw.rect(screen, (116, 171, 144), (120, 340, 60, 60))
    t = sys_font.render("2", False, WHITE)
    screen.blit(t, (135,355))

    #Cria Botão 9 (contém numero 3)
    pygame.draw.rect(screen, (116, 171, 144), (190, 340, 60, 60))
    t = sys_font.render("3", False, WHITE)
    screen.blit(t, (205,355))

    #Cria Botão 10 (contém x)
    pygame.draw.rect(screen, (255,177,69), (50, 410, 60, 60))
    t = sys_font.render("C", False, WHITE)
    screen.blit(t, (65,425))

    #Cria Botão 11 (contém numero 3)
    pygame.draw.rect(screen, (116, 171, 144), (120, 410, 60, 60))
    t = sys_font.render("0", False, WHITE)
    screen.blit(t, (135,425))
    
    #Cria Botão 12 (contém numero 3)
    pygame.draw.rect(screen, (255,177,69), (190, 410, 60, 60))
    t = sys_font.render(".", False, WHITE)
    screen.blit(t, (205,425))

    #Cria Botão 3 (contém operador *)
    pygame.draw.rect(screen, (255,177,69), (260, 200, 60, 60))
    t = sys_font.render("+", False, WHITE)
    screen.blit(t, (275,215))

    #Cria Botão 3 (contém operador +)
    pygame.draw.rect(screen, (255,177,69), (330, 200, 60, 60))
    t = sys_font.render("-", False, WHITE)
    screen.blit(t, (345,215))

    pygame.draw.rect(screen, (255,177,69), (260, 270, 60, 60))
    t = sys_font.render("*", False, WHITE)
    screen.blit(t, (275,285))

    #Cria Botão 3 (contém operador +)
    pygame.draw.rect(screen, (255,177,69), (330, 270, 60, 60))
    t = sys_font.render("/", False, WHITE)
    screen.blit(t, (345,285))

    pygame.draw.rect(screen, (255,177,69), (260, 340, 60, 60))
    t = sys_font.render("//", False, WHITE)
    screen.blit(t, (275,355))

    #Cria Botão 3 (contém operador +)
    pygame.draw.rect(screen, (255,177,69), (330, 340, 60, 60))
    t = sys_font.render("%", False, WHITE)
    screen.blit(t, (345,355))

    pygame.draw.rect(screen, (255,177,69), (260, 410, 60, 60))
    t = sys_font.render("**", False, WHITE)
    screen.blit(t, (275,425))

    #Cria Botão 3 (contém operador +)
    pygame.draw.rect(screen, (116, 171, 144), (330, 410, 60, 60))
    t = sys_font.render("=", False, WHITE)
    screen.blit(t, (345,425))

def mouse_click_down(px_mouse, py_mouse, mouse_buttons):
    global result
    if mouse_buttons[0]:
        if check_click(50, 200, 60, 60, px_mouse, py_mouse):
            result = result + "7"
        if check_click(120, 200, 60, 60, px_mouse, py_mouse):
            result = result + "8"
        if check_click(190, 200, 60, 60, px_mouse, py_mouse):
            result = result + "9"
        if check_click(50, 270, 60, 60, px_mouse, py_mouse):
            result = result + "4"
        if check_click(120, 270, 60, 60, px_mouse, py_mouse):
            result = result + "5"
        if check_click(190, 270, 60, 60, px_mouse, py_mouse):
            result = result + "6"
        if check_click(50, 340, 60, 60, px_mouse, py_mouse):
            result = result + "1"
        if check_click(120, 340, 60, 60, px_mouse, py_mouse):
            result = result + "2"
        if check_click(190, 340, 60, 60, px_mouse, py_mouse):
            result = result + "3"
        if check_click(120, 410, 60, 60, px_mouse, py_mouse):
            result = result + "0"
        if check_click(260, 270, 60, 60, px_mouse, py_mouse):
            result = result + "*"  
        if check_click(260, 200, 60, 60, px_mouse, py_mouse):
            result = result + "+"   
        if check_click(330, 200, 60, 60, px_mouse, py_mouse):
            result = result + "-"   
        if check_click(330, 270, 60, 60, px_mouse, py_mouse):
            result = result + "/"   
        if check_click(260, 340, 60, 60, px_mouse, py_mouse):
            result = result + "//"   
        if check_click(330, 340, 60, 60, px_mouse, py_mouse):
            result = result + "%"   
        if check_click(260, 410, 60, 60, px_mouse, py_mouse):
            result = result + "**"   
        if check_click(190, 410, 60, 60, px_mouse, py_mouse):
            result = result + "."   
        if check_click(50, 410, 60, 60, px_mouse, py_mouse):
            result = result[0:len(result)-1]   
        if check_click(330, 410, 60, 60, px_mouse, py_mouse):
            try:
                result = str(round(eval(result), 2))
            except:
                result = 'ERROR'
               
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
