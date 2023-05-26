import pygame
import math

width = 800  #Largura Janela
height = 600 #Altura Janela

def load():
    global clock, bola, rectangle1,rectangle2
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



def update(dt):
    global  rectangle1, rectangle2, bola

    keys = pygame.key.get_pressed()
    bolean = True
    bola['x'] = bola['x'] - (dt * bola['speed'])
    


    if bola['y']+10 >= rectangle2['y'] and bola['y'] + 10 <= rectangle2['y']+100 and bola['x'] >= rectangle2['x'] and bola['x'] <= rectangle2['x']+30: 
        print('bateu')

    if bola['y']+10 >= rectangle1['y'] and bola['y'] + 10 <= rectangle1['y']+100 and bola['x'] >= rectangle1['x'] and bola['x'] <= rectangle1['x']+30: 
        bola['x'] += dt * bola['speed']

    if keys[pygame.K_UP]:
        if rectangle2['y'] >= 10:
            rectangle2['y'] -= (dt*0.5)
        if rectangle1['y'] >= 10:
            rectangle1['y'] -= (dt*0.5)
    
    if keys[pygame.K_DOWN]:
        if rectangle2['y'] <= (590 - rectangle2['h']):
            rectangle2['y'] += (dt*0.5)
        if rectangle1['y'] <= (590 - rectangle1['h']) :
            rectangle1['y'] += (dt*0.5)


    



    ##player1["collided"] = False

'''  if check_circular_collision(player1["x"], player1["y"], player1["raio"], 
                           circle1["x"], circle1["y"], circle1["raio"]):
        player1["collided"] = True
        circle1["collided"] = True
    else:
        circle1["collided"] = False

        
    if not check_circular_collision(player1["x"] + dx_player, player1["y"] + dy_player, player1["raio"], 
                           circle1["x"], circle1["y"], circle1["raio"]):    
        player1["x"] = player1["x"] + dx_player
        player1["y"] = player1["y"] + dy_player
        circle1["collided"] = False
    
    else:
        player1["collided"] = True
        circle1["collided"] = True
        '''
            
        
        
        

def draw_screen(screen):
    screen.fill((0,0,0))
    
    
    color_circle1 = (255,255,255)
   ## if circle1["collided"] == True:
   ##     color_circle1 = (0,0,255)
    
   ## pygame.draw.circle(screen,color_circle1, (circle1["x"], circle1["y"]), circle1["raio"])
    
 
    pygame.draw.rect(screen, (255,255,255), pygame.Rect(rectangle1['x'], rectangle1['y'], rectangle1['w'], rectangle1['h']))
    pygame.draw.rect(screen, (255,255,255), pygame.Rect(rectangle2['x'], rectangle2['y'], rectangle2['w'], rectangle2['h'])) 




    
    color_player = (255,0,0)
    ##if player1["collided"] == True:
      ##  color_player = (255,0,0)


    pygame.draw.circle(screen, color_player, (bola["x"], bola["y"]), bola["raio"])





def main_loop(screen):  
    global clock
    running = True
    while running:
        for e in pygame.event.get(): 
            if e.type == pygame.QUIT:
                running = False
                break
        
        clock.tick(60) 			# Define FPS máximo
 
        # Calcula tempo transcorrido desde
        # a última atualização 
        dt = clock.get_time()
         
        draw_screen(screen)			 # Desenha objetos na tela
        
        update(dt) 			# Atualiza posição dos objetos da tela

        pygame.display.update() 	# Pygame atualiza o seu estado


pygame.init()
screen = pygame.display.set_mode((width, height))
load()
main_loop(screen)
pygame.quit()
