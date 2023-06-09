import pygame

width = 800  #Largura Janela
height = 600 #Altura Janela

def load():
    global clock, bola, rectangle1,rectangle2, voce_perdeu
    clock = pygame.time.Clock() 
    bola = {
      "x": 390,
      "y": 300,
      "raio": 10,
      "speed": 0.2,
    }

    rectangle1 = {
        'x': 30,
        'y': 30,
        'w': 30,
        'h': 100 
    }
    rectangle2 = {
        'x': 740,
        'y': 30,
        'w': 30,
        'h': 100 
    }

    def voce_perdeu():
        screen.fill((0, 0, 0))
        font_size = 40
        font_color = (255, 255, 255)
        font = pygame.font.Font(None, font_size)
        text_content = "Game Over!"
        text_surface = font.render(text_content, True, font_color)
        text_rect = text_surface.get_rect(center=(width // 2, height//2))
        screen.blit(text_surface, text_rect)

def update(dt):
    global  rectangle1, rectangle2, bola, voce_perdeu
    keys = pygame.key.get_pressed()
    bola['x'] += dt * bola['speed']

    
    if bola['y']+10 >= rectangle2['y'] and bola['y'] + 10 <= rectangle2['y']+100 and bola['x'] + 10 >= rectangle2['x']  and bola['x'] + 10 <= rectangle2['x']+30: 
        bola['speed'] = -bola['speed']
    if bola['y']+10 >= rectangle1['y'] and bola['y'] + 10 <= rectangle1['y']+100 and bola['x'] - 10 >= rectangle1['x'] and bola['x'] - 10 <= rectangle1['x']+30: 
        bola['speed'] = -bola['speed']


    if bola['x'] >= 800 or bola['x'] <= 0:
        voce_perdeu()


    if keys[pygame.K_w]:
        if rectangle1['y'] >= 10:
            rectangle1['y'] -= (dt*0.5)
        
    if keys[pygame.K_UP]:
        if rectangle2['y'] >= 10:
            rectangle2['y'] -= (dt*0.5)
    if keys[pygame.K_s]:
        if rectangle1['y'] <= (590 - rectangle1['h']) :
            rectangle1['y'] += (dt*0.5)
    if keys[pygame.K_DOWN]:
        if rectangle2['y'] <= (590 - rectangle2['h']):
            rectangle2['y'] += (dt*0.5)
        

def draw_screen(screen):
    screen.fill((0,0,0))
    pygame.draw.rect(screen, (255,255,255), pygame.Rect(rectangle1['x'], rectangle1['y'], rectangle1['w'], rectangle1['h']))
    pygame.draw.rect(screen, (255,255,255), pygame.Rect(rectangle2['x'], rectangle2['y'], rectangle2['w'], rectangle2['h'])) 
    pygame.draw.circle(screen, (255,0,0), (bola["x"], bola["y"]), bola["raio"])

def main_loop(screen):  
    global clock, dt
    running = True
    while running:
        for e in pygame.event.get(): 
            if e.type == pygame.QUIT:
                running = False
                break

        clock.tick(60)
        dt = clock.get_time()
        draw_screen(screen)
        update(dt)
        pygame.display.update()

pygame.init()
screen = pygame.display.set_mode((width, height))
load()
main_loop(screen)
event = pygame.event.wait()


pygame.quit()