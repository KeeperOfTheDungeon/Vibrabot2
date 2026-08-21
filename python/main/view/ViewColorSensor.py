import hardware.ColorSensor
from view.SensorView import SensorView
import pygame as pygame

class ViewColorSensor(SensorView):
    def __init__(self,x_pos, y_pos, color_sensor):
        super().__init__(x_pos, y_pos,190, 160,color_sensor.get_name())
        self.color_sensor = color_sensor 
        pass

    def draw(self):
        super().draw()





        yPos = 30 
        for index in range(0,5):
            intensity = self.color_sensor.get_intensity(index)
            intensity_string = f"{intensity:.3f}"
            sensor_name = self.color_sensor.get_sensor_name(index)

            text = self.font.render(
                sensor_name,
                True,
                (255, 255, 255)
            )
            self.blit(text, (10, yPos))

            text = self.font.render(
                " : "+intensity_string,
                True,
                (255, 255, 255)
            )
            self.blit(text, (50, yPos))

            yPos = yPos + 20


        red = self.color_sensor.get_intensity(1)
        green = self.color_sensor.get_intensity(2)
        blue = self.color_sensor.get_intensity(3)

        max_color = red

        if red < blue:
            max_color = blue
            
        if  green > max_color:
            max_color = green

        if max_color == 0:
            max_color = 0.001

        faktor = (1.0 / max_color) *100
        
        red = int (red * faktor)
        green = int (green * faktor)
        blue = int (blue * faktor)
        
        pygame.draw.rect(self, (red,green,blue),(130,30,50,100))


