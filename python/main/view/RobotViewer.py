import pygame as pygame
from ui.window_manager import WindowManager

class RobotViewer:
    def __init__(self, robot):
        self.robot = robot
        self.window_manager = WindowManager()
        

    def draw(self, screen):
        self.window_manager.draw(screen)

    def on_mouse_left(self, position):
        self.window_manager.on_mouse_left(position)
