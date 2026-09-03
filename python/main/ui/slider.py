import hardware.light_sensor
from ui.ui_element import UiElement
import pygame as pygame

class Slider(UiElement):
    def __init__(self,x_pos, y_pos, x_size, y_size, name):
        super().__init__(x_pos, y_pos,x_size, y_size, name)
        self.bkgColor = (200, 200, 40)
        self.font = pygame.font.Font(None, 24)
        self.extend = 0
        self.value = 0.0
        self.listener_list = list()

    def draw(self,context):
        super().draw(context)
        pygame.draw.rect(context, (255,0,0),(self.x_pos,self.y_pos,self.x_size,self.y_size),2)

        x_pos = self.x_pos + 1
        y_pos = self.y_size - self.extend + self.y_pos
        x_extend = self.x_size - 2 
        y_extend = self.extend -2
        pygame.draw.rect(
                         context,
                         (150,50,50),
                         pygame.Rect(x_pos, y_pos , x_extend, y_extend) ) 


    def on_mouse_left(self,point):
        super().on_mouse_left(point)
        self.extend = self.y_size- 1 -point[1]
        self.value = float(self.extend / (self.y_size -1))
        print(self.value)
        for listener in self.listener_list:
            listener.slider_value_change(self)

    def add_listener(self, listener):
        self.listener_list.append(listener)

    def get_value(self):
        return (self.value)