import asyncio
from hardware.Vibrabot import Vibrabot
from hardware.light_sensor import Light_sensor
from hardware.AudioSensor import AudioSensor
from hardware.ProximitySensor import ProximitySensor
from hardware.ColorSensor import ColorSensor
from hardware.AudioSpectrum import AudioSpectrum
from hardware.motor import Motor
from ui.window_manager import WindowManager
from view.VibrabotViewer import VibrabotViewer
from view.ViewColorSensor import ViewColorSensor
from view.ViewAudioSensor import ViewAudioSensor
from view.ProximitySensorView import ProximitySensorView
from view.MotorView import MotorView
from view.view_light_sensor import view_light_sensor


import pygame
import math
import numpy as np
from ble.ble import Ble
import threading
import queue

# ----------------------------
# Einstellungen
# ----------------------------

WIDTH = 1000
HEIGHT = 900

ROBOT_RADIUS = 15
SENSOR_LENGTH = 120

FPS = 60


# ----------------------------
# Roboter
# ----------------------------

class Robot_test:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.angle = 0

        self.speed_left = 0
        self.speed_right = 0
        


    def update(self, obstacles):

        # Save previous position
        old_x = self.x
        old_y = self.y
        old_angle = self.angle

        # Differential drive
        speed = (self.speed_left + self.speed_right) / 2
        rotation = (self.speed_right - self.speed_left) / 40

        self.angle += rotation

        self.x += math.cos(self.angle) * speed
        self.y += math.sin(self.angle) * speed

        # Collision?
        if self.check_collision(obstacles):
            self.x = old_x
            self.y = old_y
            self.angle = old_angle

    def sensors(self, obstacles):

        values = []
        self.sensor_lines = []

        for offset in [-0.8, 0, 0.8]:

            angle = self.angle + offset

            distance = SENSOR_LENGTH

            for d in range(SENSOR_LENGTH):

                sx = self.x + math.cos(angle) * d
                sy = self.y + math.sin(angle) * d

                hit = False

                for rect in obstacles:
                    if rect.collidepoint(sx, sy):
                        distance = d
                        hit = True
                        break

                if hit:
                    break

            values.append(distance / SENSOR_LENGTH)

            self.sensor_lines.append(
                (
                    (self.x, self.y),
                    (
                        self.x + math.cos(angle) * distance,
                        self.y + math.sin(angle) * distance
                    ),
                    distance
                )
            )

        return np.array(values)

    def draw(self, screen):

        pygame.draw.circle(
            screen,
            (0,200,0),
            (int(self.x), int(self.y)),
            ROBOT_RADIUS
        )

        # Richtung anzeigen
        x2 = self.x + math.cos(self.angle)*30
        y2 = self.y + math.sin(self.angle)*30

        pygame.draw.line(
            screen,
            (255,255,0),
            (self.x,self.y),
            (x2,y2),
            3)
            
        # Draw sensors
        
        for start, end, distance in self.sensor_lines:

            if distance < SENSOR_LENGTH:
                color = (255, 0, 0)      # obstacle detected
            else:
                color = (0, 255, 255)    # clear

            pygame.draw.line(screen, color, start, end, 2)
            pygame.draw.circle(screen, color,(int(end[0]), int(end[1])), 4 )


    def check_collision(self, obstacles):

        for rect in obstacles:

            closest_x = max(rect.left, min(self.x, rect.right))
            closest_y = max(rect.top, min(self.y, rect.bottom))

            dx = self.x - closest_x
            dy = self.y - closest_y

            if dx * dx + dy * dy < ROBOT_RADIUS * ROBOT_RADIUS:
                return True

        return False
	
# ----------------------------
# Welt
# ----------------------------

rx_queue = queue.Queue()
tx_queue = queue.Queue()

ls = Light_sensor("left eye")
ls2 = Light_sensor("right eye")
color_sensor = ColorSensor("color Sensor") 
audio_sensor =AudioSensor("Spectrum")

left_proximity_sensor = ProximitySensor("left proximity")
center_proximity_sensor = ProximitySensor("center proximity")
right_proximity_sensor = ProximitySensor("right proximity")

left_motor = Motor("left")
right_motor = Motor("right")


window_manager = WindowManager()


def decode_ble_package(package):
        value = int.from_bytes(package[0:1], byteorder='little')

        #print (value)

        match  value:
            case  0xa0:
                decode_ble_light_Package(package)
            case 0xa1:
                decode_ble_proximity_Package(package)
            case 0xB0:
                decode_ble_fft_Package(package)



def decode_ble_fft_Package(package):
    spectrum = AudioSpectrum()
    position = 2
    
    for index in  range(3):
        bin = int.from_bytes(package[position :position +2], byteorder='little')

        level = int.from_bytes(package[position+2 :position +4], byteorder='little')
        spectrum.set_bin(index, bin,level)
        position = position + 4

    audio_sensor.add(spectrum)



def decode_ble_light_Package(package):
    value = int.from_bytes(package[2:4], byteorder='little')
    f = float(value)/4906
    ls.set_intensity(f )  
    
    value = int.from_bytes(package[4:6], byteorder='little')
    f = float(value)/4906
    ls2.set_intensity(f )  

    value = int.from_bytes(package[6:8], byteorder='little')
    f = float(value)/65536
    color_sensor.set_intensity(0,f)  

    value = int.from_bytes(package[8:10], byteorder='little')
    f = float(value)/65536
    f = f * (34.0 / 41.0)
    color_sensor.set_intensity(1,f)  

    value = int.from_bytes(package[10:12], byteorder='little')
    f = float(value)/65536
    f = f * (34.0 / 39.0)
    color_sensor.set_intensity(2,f)  

    value = int.from_bytes(package[12:14], byteorder='little')
    f = float(value)/65536
    color_sensor.set_intensity(3,f)  

    value = int.from_bytes(package[14:16], byteorder='little')
    f = float(value)/65536
    color_sensor.set_intensity(4,f)  



def decode_ble_proximity_Package(package):

    position = 2

    value  = int.from_bytes(package[position :position +2], byteorder='little')
    f = float(value)/4906
    left_proximity_sensor.set_intensity(f)

    position += 2
    value  = int.from_bytes(package[position :position +2], byteorder='little')
    f = float(value)/4906
    center_proximity_sensor.set_intensity(f)

    position += 2
    value  = int.from_bytes(package[position :position +2], byteorder='little')
    f = float(value)/4906
    right_proximity_sensor.set_intensity(f)

    position += 2
    value  = int.from_bytes(package[position :position +2], byteorder='little')
    f = float(value)
     
    left_proximity_sensor.set_status(f)
    center_proximity_sensor.set_status(f)
    right_proximity_sensor.set_status(f)
    
   


async def main():


   

    pygame.init()

    screen = pygame.display.set_mode(
        (WIDTH, HEIGHT)
    )

  #  asyncio.create_task(ble_task())

    clock = pygame.time.Clock()


    obstacles = [
        pygame.Rect(300,200,100,40),
        pygame.Rect(500,400,150,50),
        pygame.Rect(200,450,80,80)
    ]

    vibrabot = Vibrabot("vibrabot")
    vibrabot_viever = VibrabotViewer(vibrabot)

    robot = Robot_test(100,100)
# light sensor
   # left_light_sensor_view = view_light_sensor(696, 10,ls)
   # right_light_sensor_view = view_light_sensor(848, 10,ls2)
   # color_sensor_view = ViewColorSensor(696, 478, color_sensor)
   # audio_sensor_view = ViewAudioSensor(696, 650, audio_sensor)

   # left_proximity_sensor_view = ProximitySensorView(696, 202, left_proximity_sensor)
   # center_proximity_sensor_view = ProximitySensorView(696, 294, center_proximity_sensor)
   # right_proximity_sensor_view = ProximitySensorView(696, 386, right_proximity_sensor)
   # motor_view = MotorView(886, 478, left_motor, right_motor)

    # ----------------------------
    # Hauptschleife
    # ----------------------------

   # window_manager.add(left_light_sensor_view)
   # window_manager.add(right_light_sensor_view)

   # window_manager.add(left_proximity_sensor_view)
   # window_manager.add(center_proximity_sensor_view)
   # window_manager.add(right_proximity_sensor_view)

    #window_manager.add(color_sensor_view)
   # window_manager.add(audio_sensor_view)
   # window_manager.add(motor_view)

    running = True

    while running:

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running=False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                print("Mouse button:", event.button)
                print("Position:", event.pos)
                vibrabot_viever.on_mouse_left(event.pos)

            


        # Teststeuerung
        keys = pygame.key.get_pressed()

        robot.speed_left = 0
        robot.speed_right = 0

        if keys[pygame.K_UP]:
            robot.speed_left = 3
            robot.speed_right = 3

        if keys[pygame.K_LEFT]:
            robot.speed_left = -1
            robot.speed_right = 3

        if keys[pygame.K_RIGHT]:
            robot.speed_left = 3
            robot.speed_right = -1


        robot.update(obstacles)


        # Sensorwerte
        s = robot.sensors(obstacles)

    # print(s)
     

        # Zeichnen
        screen.fill((30,30,30))


        for rect in obstacles:
            pygame.draw.rect(
                screen,
                (150,50,50),
                rect
            )


        robot.draw(screen)
        vibrabot_viever.draw(screen)
        #window_manager
        pygame.draw.circle(screen, (0,200,0),((300, 300)), 4 )

    
        pygame.display.flip()


     



        clock.tick(FPS)
        await asyncio.sleep(0)

    ###### FiFO

        if not rx_queue.empty():
            data = rx_queue.get()
            decode_ble_package(data)


    pygame.quit()

asyncio.run(main())    
