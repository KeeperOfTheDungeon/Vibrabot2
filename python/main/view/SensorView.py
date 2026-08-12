from pygame import Surface
import pygame as pygame

class SensorView(Surface):
    def __init__(self, width, height, name):
        super().__init__((width, height))
        
        self.width = width
        self.height = height
        self.font = pygame.font.Font(None, 24)
        self.name = name

    def draw(self):
        self.fill((20, 20, 20))

#        for i in range(len(values) - 1):
 #           pygame.draw.line(
  #              self,
   #             (0, 255, 0),
    #            (i, self.height // 2 - values[i]),
     #           (i + 1, self.height // 2 - values[i + 1])
      #      )

        text = self.font.render(
            self.name,
            True,
            (255, 255, 255)
        )

        self.blit(text, (10, 10))



