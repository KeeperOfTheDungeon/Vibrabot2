import pygame
import math
import numpy as np

# ----------------------------
# Einstellungen
# ----------------------------

WIDTH = 800
HEIGHT = 600

ROBOT_RADIUS = 15
SENSOR_LENGTH = 120

FPS = 60


# ----------------------------
# Roboter
# ----------------------------

class Robot:
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

pygame.init()

screen = pygame.display.set_mode(
    (WIDTH, HEIGHT)
)

clock = pygame.time.Clock()


obstacles = [
    pygame.Rect(300,200,100,40),
    pygame.Rect(500,400,150,50),
    pygame.Rect(200,450,80,80)
]


robot = Robot(100,100)


# ----------------------------
# Hauptschleife
# ----------------------------

running = True

while running:

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running=False


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

    print(s)


    # Zeichnen
    screen.fill((30,30,30))


    for rect in obstacles:
        pygame.draw.rect(
            screen,
            (150,50,50),
            rect
        )


    robot.draw(screen)


    pygame.display.flip()

    clock.tick(FPS)


pygame.quit()