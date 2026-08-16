import hardware.AudioSensor
from view.SensorView import SensorView
import pygame as pygame

class ViewAudioSensor(SensorView):
    def __init__(self, audio_sensor):
        super().__init__(300, 300,audio_sensor.get_name())
        self.audio_sensor = audio_sensor 


    def draw(self):
        super().draw()

        levels = [0] * 127
        last_spectrum =self.audio_sensor.get_last()
        
        print("bands")
        for i in range (3):
            index = int(last_spectrum.bins[i]/100); 
            levels[index] = int(last_spectrum.levels[i]/25)
            print(levels[index])


            
        for index in range (127):
            pygame.draw.line(
                            self,
                            (0, 255, 0),
                            (10, 20 + (2*index)),
                            (10 + levels[index], 20+(2*index))
                        )
            



      #  pygame.draw.rect(self, (red,green,blue),(9,150,100,20))

