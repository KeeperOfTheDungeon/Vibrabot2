from pygame import Surface
import pygame as pygame

class SensorView(Surface):
    def __init__(self, x_pos, y_pos, width, height, name):
        super().__init__((width, height))
        
        self.width = width
        self.height = height
        self.font = pygame.font.Font(None, 24)
        self.name = name
        self.x_pos = x_pos
        self.y_pos = y_pos

    def set_pos(self, x_pos, y_pos):
        self.x_pos = x_pos
        self.y_pos = y_pos

    def get_x_pos(self):
        return (self.x_pos)

    def get_y_pos(self):
        return (self.y_pos)

    def get_name(self):
        return (self.name)

    def contains_point(self, point):
        x_pos = point[0]
        y_pos = point[1]

        if (self.x_pos > x_pos):
            return (False)
        
        if ((self.x_pos+ self.width) < x_pos):
            return (False)
        
        if (self.y_pos > y_pos):
            return (False)
        
        if ((self.y_pos+ self.height) < y_pos):
            return (False)

        return(True)

    def on_mouse(self,point):
        x_pos = point[0] -self.x_pos
        y_pos = point[1]- self.y_pos

        print(x_pos)
        print(y_pos)

    def draw(self):
        self.fill((40, 40, 40))
        
        pygame.draw.rect(self, (255, 255, 255), (0, 0, self.width, self.height), 2)
        pygame.draw.rect(self, (0, 0, 255), (2, 2, self.width-4, 26), 14)

        text = self.font.render(
            self.name,
            True,
            (255, 255, 255)
        )

        self.blit(text, (10, 8))



