#textureAtlas
import pygame
import math
tile_size = 32
width = 32*20
height = 32*20
mapa = []
tile_quads = []
background_image = pygame.image.load("fundo.png")
imagem_bala = pygame.image.load("bala.png")
contagem_tempo = 180000
nave = {
    'imagem': 'nave.png',
    'x': 16*20,
    'y': 25*20,
    'velocidade': 0.6,
    'colisao': False
}


class Bala:
    def __init__ (self, velocidade):
        self.velocidade = velocidade
        self.y = 600
    
    def update(self, dt):
        self.y -= dt
        screen.blit(imagem_bala, (nave['x']+40, self.y))
        print(self.y)

        

def load_mapa(filename): #Lê o conteúdo do arquivo para a matriz
    global mapa
    file = open(filename,"r")
    i = 0 
    for line in file.readlines():
        mapa.append([])
        for j in line:
            mapa[i].append(j)
        i = i + 1
    file.close()
    
def load_tiles(filename, nx, ny):
    global tileset_image, tile_quads
    tileset_image = pygame.image.load(filename)
    for i in range(nx):
        for j in range(ny):
            tile_quads.append((i * tile_size , j * tile_size, tile_size, tile_size))


def load():
    global clock, plataform_tileset
    clock = pygame.time.Clock() 
    load_mapa("plataform_map.txt")
    load_tiles("plataform_tileset.png", 3, 3)
    
def update(dt):
    global nave, contagem_tempo
    contagem_tempo -= dt
    keys = pygame.key.get_pressed()

    if keys[pygame.K_UP]:
        if nave['y'] <= 0:
            nave['y'] = 0
        else:
            nave['y'] = nave['y'] - (nave['velocidade'] * dt)
    if keys[pygame.K_DOWN]:
        if nave['y'] >= 550:
            nave['y'] = 550
        else:
            nave['y'] = nave['y'] + (nave['velocidade'] * dt)

    if keys[pygame.K_LEFT]:
        if nave['x'] <= 0:
            nave['x'] = 0
        else:
            nave['x'] = nave['x'] - (nave['velocidade'] * dt)
    if keys[pygame.K_RIGHT]:
        if nave['x'] >= 560:
            nave['x'] = 560
        else:
            nave['x'] = nave['x'] + (nave['velocidade'] * dt)
    
    if keys[pygame.K_SPACE]:
        Bala(0.10).update(dt)

def draw_screen(screen):
    screen.blit(background_image, (0,0))
    screen.blit(pygame.image.load(nave['imagem']), (nave['x'], nave['y']))
    
    #screen.fill((250,250,250))
    # for i in range(10): #Percorre a matriz e desenha imagens quadradas
    #     for j in range(14):
    #         pos = ((j * tile_size), (i * tile_size))
    #         if mapa[i][j] == "G":
    #             screen.blit(tileset_image, pos, tile_quads[0])
    #         elif mapa[i][j] == "T":
    #             screen.blit(tileset_image, pos, tile_quads[3])
    #         elif mapa[i][j] == "A": 
    #             screen.blit(tileset_image, pos, tile_quads[6])
    #         elif mapa[i][j] == "P": 
    #             screen.blit(tileset_image, pos, tile_quads[7])
    #         elif mapa[i][j] == "B": 
    #             screen.blit(tileset_image, pos, tile_quads[5])

def main_loop(screen):  
    global clock, contagem_tempo
    font = pygame.font.Font(None, 30)
    running = True
    while running:
        for e in pygame.event.get(): 
            if e.type == pygame.QUIT:
                running = False
                break
        
        # Define FPS máximo
        clock.tick(60)        
        # Calcula tempo transcorrido desde a última atualização 
        dt = clock.get_time()
        # Desenha objetos na tela 
        draw_screen(screen)
        # Atualiza posição dos objetos da tela
        update(dt)
        # Pygame atualiza o seu estado
        t = contagem_tempo/1000
        text = font.render(str(math.floor(t)), True, (255,255,255))
        text_rect = text.get_rect(center = (320, 20))
        screen.blit(text, text_rect)
        pygame.display.update() 

pygame.init()
pygame.mixer.init()
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Navezinha BEBOP")
load()
main_loop(screen)
pygame.quit()
