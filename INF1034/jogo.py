import pygame
import math
tile_size = 32
width = 32*20
height = 32*20
background_image = pygame.image.load("./INF1034/fundo.png")
contagem_tempo = 180000

class Nave(pygame.sprite.Sprite):
	def __init__(self, x, y):
		pygame.sprite.Sprite.__init__(self)
		self.image = pygame.image.load("./INF1034/nave.png")
		self.rect = self.image.get_rect()
		self.rect.center = [x, y]
		self.last_shot = pygame.time.get_ticks()


	def update(self):
		speed = 8
		key = pygame.key.get_pressed()
		if key[pygame.K_LEFT] and self.rect.left > 0:
			self.rect.x -= speed
		if key[pygame.K_RIGHT] and self.rect.right < width:
			self.rect.x += speed
			
                        
		time_now = pygame.time.get_ticks()
		#shoot
		if key[pygame.K_SPACE]:
			bala = Bullets(self.rect.centerx, self.rect.top)
			grupo_bala.add(bala)
			self.last_shot = time_now


class Bullets(pygame.sprite.Sprite):
	def __init__(self, x, y):
		pygame.sprite.Sprite.__init__(self)
		self.image = pygame.image.load("./INF1034/bala.png")
		self.rect = self.image.get_rect()
		self.rect.center = [x, y]

	def update(self):
		self.rect.y -= 5
		if self.rect.bottom < 0:
			self.kill()

		# self.mask = pygame.mask.from_surface(self.image)

def load():
    global clock
    clock = pygame.time.Clock() 
    
def update(dt):
    global nave, contagem_tempo
    contagem_tempo -= dt
    

def draw_screen(screen):
    screen.blit(background_image, (0,0))


grupo_bala = pygame.sprite.Group()

grupo_nave = pygame.sprite.Group()
nave = Nave(int(width / 2), height - 100)
grupo_nave.add(nave)
    
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
        nave.update()
        grupo_bala.update()
        # Pygame atualiza o seu estado
        t = contagem_tempo/1000
        text = font.render(str(math.floor(t)), True, (255,255,255))
        text_rect = text.get_rect(center = (320, 20))
        screen.blit(text, text_rect)
        grupo_nave.draw(screen)
        grupo_bala.draw(screen)
        pygame.display.update() 

pygame.init()
pygame.mixer.init()
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Navezinha BEBOP")
load()
main_loop(screen)
pygame.quit()
