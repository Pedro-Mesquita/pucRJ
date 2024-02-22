import pygame
import random

width = 800  #Largura Janela
height = 600 #Altura Janela


def load():
    global sys_font
    sys_font = pygame.font.Font(pygame.font.get_default_font(), 20)

def draw_screen(screen):
    screen.fill((48,48,48))   
    #Cria quadrado
    # 
    pygame.draw.polygon(screen,(145, 145, 145), [(0,600), (800,600), (800, 500), (0, 500)])
    for x in range(10):
        pygame.draw.polygon(screen,(255, 255, 255), [(20+(x*90),550), (90+(x*90),550), (90+(x*90), 520), (20+(x*90), 520)])
    pygame.draw.polygon(screen,(137, 107, 107), [(0,500), (800,500), (800, 400), (0, 400)])
    pygame.draw.circle(screen, (255,255,255), (random.randint(0,800), random.randint(0,400)), 2)

def main_loop(screen):
    running = True
    while running:
        for e in pygame.event.get(): 
            if e.type == pygame.QUIT:
                running = False
                break
        # Desenha objetos na tela 
        draw_screen(screen)
        # Pygame atualiza o seu estado
        pygame.display.update()
        
pygame.init()
screen = pygame.display.set_mode((width, height))
load()
main_loop(screen)
pygame.quit()
