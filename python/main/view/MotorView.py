from ui.slider import Slider
from view.SensorView import SensorView
import pygame as pygame

class MotorView(SensorView):
    def __init__(self,x_pos, y_pos, motor_left,motor_right):
        super().__init__(x_pos, y_pos,100, 200,"motors")
        self.motor_left = motor_left 
        self.motor_right = motor_right 
        self.left_motor_slider = Slider(10,30,30,100,"left")
        self.right_motor_slider = Slider(50,30,30,100,"right")

        
        self.addComponent(self.left_motor_slider)
        self.left_motor_slider.add_listener(self)

        self.addComponent(self.right_motor_slider)
        self.right_motor_slider.add_listener(self)

        #self.bkgColor = (100, 100, 40)

    def draw(self):
        super().draw()
        self.left_motor_slider.draw(self)
        self.right_motor_slider.draw(self)

    def slider_value_change(self, component):
        print(component)

        


