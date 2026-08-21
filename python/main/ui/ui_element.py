import hardware.light_sensor
from view.SensorView import SensorView
import pygame as pygame

class UiElement:
    def __init__(self,x_pos, y_pos, x_size, y_size, name):
        self.x_pos = x_pos
        self.y_pos = y_pos
        self.x_size = x_size
        self.y_size = y_size
        self.name = name
    
    def draw(self):
        pass

    def set_name(self, name):
        self.name = name


    def get_name(self):
        return(self.name)

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

    def on_mouse_left(self,point):
        x_pos = point[0] -self.x_pos
        y_pos = point[1]- self.y_pos

        print(x_pos)
        print(y_pos)