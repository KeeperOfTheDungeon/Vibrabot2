import hardware.light_sensor
from view.SensorView import SensorView
import pygame as pygame

class view_light_sensor(SensorView):
    def __init__(self, light_sensor):
        super().__init__(190, 200,light_sensor.get_name())
        self.light_sensor = light_sensor 
        pass

    def draw(self):
        super().draw()


        pygame.draw.rect(self, (0,0,0),(9,50,128,128))



        history = self.light_sensor.get_history()
        values = list(history)
        prev_x = 0;
        prev_y = 0;
        actual_x = 10;
        actual_y = 180;

        for i in range(len(values) - 1):
            value = values[i]
            value = value * 128

            prev_x = actual_x
            prev_y = actual_y

            actual_x = actual_x +2
            actual_y = int(180-value)

            pygame.draw.line(
                    self,
                    (0, 255, 0),
                    (prev_x, int(prev_y)),
                    (prev_x, int(actual_y))
            )


            pygame.draw.line(
                self,
                (0, 255, 0),
                (prev_x+1, int(actual_y)),
                (prev_x+1, int(actual_y+1))
            )




        intensity = self.light_sensor.get_intensity()
        intensity_string = f"{intensity:.3f}"
        text = self.font.render(
            intensity_string,
            True,
            (255, 255, 255)
        )

        self.blit(text, (10, 30))


