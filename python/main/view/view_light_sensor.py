import hardware.light_sensor
from view.SensorView import SensorView
import pygame as pygame

class view_light_sensor(SensorView):
    def __init__(self, light_sensor):
        super().__init__(200, 200,light_sensor.get_name())
        self.light_sensor = light_sensor 
        pass

    def draw(self):
        super().draw()


        pygame.draw.rect(self, (200,200,200),(9,50,128,128))



        history = self.light_sensor.get_history()
        values = list(history)


        for i in range(len(values) - 1):
            value = values[i]
            value = value * 128


            pygame.draw.line(
                self,
                (0, 255, 0),
                (10+i*2, int(180-value)),
                (10+i*2+1, int(180-value))
            )




        intensity = self.light_sensor.get_intensity()
        intensity_string = f"{intensity:.3f}"
        text = self.font.render(
            intensity_string,
            True,
            (255, 255, 255)
        )

        self.blit(text, (10, 30))


