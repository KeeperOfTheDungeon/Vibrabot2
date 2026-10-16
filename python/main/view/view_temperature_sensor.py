import hardware.light_sensor
from view.SensorView import SensorView
import pygame as pygame

class ViewTemperatureSensor(SensorView):
    def __init__(self,x_pos, y_pos, temperaure_sensor):
        super().__init__(x_pos, y_pos,150, 190,temperaure_sensor.get_name())
        self.temperaure_sensor = temperaure_sensor 
        pass

    def draw(self):
        super().draw()


        capacity = self.temperaure_sensor.get_temperature()
        capacity_string = f"{capacity:.1f}"
        text = self.font.render(
            capacity_string,
            True,
            (255, 255, 255)
        )

        self.blit(text, (10, 30))


