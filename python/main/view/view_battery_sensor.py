import hardware.light_sensor
from view.SensorView import SensorView
import pygame as pygame

class ViewBatterySensor(SensorView):
    def __init__(self,x_pos, y_pos, battery_sensor):
        super().__init__(x_pos, y_pos,150, 190,battery_sensor.get_name())
        self.battery_sensor = battery_sensor 
        pass

    def draw(self):
        super().draw()


        capacity = self.battery_sensor.get_capacity()
        capacity_string = f"{capacity:.3f}"
        text = self.font.render(
            capacity_string,
            True,
            (255, 255, 255)
        )

        self.blit(text, (10, 30))


