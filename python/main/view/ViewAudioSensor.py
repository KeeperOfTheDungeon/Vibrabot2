import hardware.AudioSensor
from view.SensorView import SensorView
import pygame as pygame

class ViewAudioSensor(SensorView):
    def __init__(self, x_pos, y_pos, audio_sensor):
        super().__init__(x_pos, y_pos,300, 160,audio_sensor.get_name())
        self.audio_sensor = audio_sensor 


    def draw(self):
        super().draw()

        levels = [0] * 127
        last_spectrum =self.audio_sensor.get_last()

        for i in range (3):
            index = int(last_spectrum.bins[i]/100); 
            levels[index] = int(last_spectrum.levels[i]/25)
            
        for index in range (127):
            pygame.draw.line(
                            self,
                            (0, 255, 0),
                            (10, 30 + index),
                            (10 + levels[index], 30+index)
            )
            


