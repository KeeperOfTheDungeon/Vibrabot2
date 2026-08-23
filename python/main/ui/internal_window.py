from pygame import Surface
import pygame as pygame

class InternalWindow(Surface):
    def __init__(self,x_pos, y_pos, x_size, y_size, name):
        super().__init__((x_size, y_size))
        self.x_pos = x_pos
        self.y_pos = y_pos
        self.x_size = x_size
        self.y_size = y_size
        self.name = name
        self.bkgColor = (100, 100, 40)

    def set_pos(self, x_pos, y_pos):
        self.x_pos = x_pos
        self.y_pos = y_possel

    
    def draw(self):
        self.fill(self.bkgColor)



    def get_x_pos(self):
        return (self.x_pos)

    def get_y_pos(self):
        return (self.y_pos)

    
    def set_name(self, name):
        self.name = name


    def get_name(self):
        return(self.name)

    def contains_point(self, point):
            x_pos = point[0]
            y_pos = point[1]

            if (self.x_pos > x_pos):
                return (False)
            
            if ((self.x_pos+ self.x_size) < x_pos):
                return (False)
            
            if (self.y_pos > y_pos):
                return (False)
            
            if ((self.y_pos+ self.y_size) < y_pos):
                return (False)

            return(True)

    def on_mouse_left(self,point):
        x_pos = point[0] -self.x_pos
        y_pos = point[1]- self.y_pos
        print("internal")
        print(x_pos)
        print(y_pos)