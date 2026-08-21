import hardware.light_sensor
from view.SensorView import SensorView
import pygame as pygame

class ProximitySensorView(SensorView):
    def __init__(self,x_pos, y_pos, proximity_sensor):
        super().__init__(x_pos, y_pos,300, 90,proximity_sensor.get_name())
        self.proximity_sensor = proximity_sensor 
        pass

    def draw(self):
        super().draw()

        intensity = self.proximity_sensor.get_intensity()
        intensity_string = f"{intensity:.3f}"
        text = self.font.render(
            intensity_string,
            True,
            (255, 255, 255)
        )
        self.blit(text, (10, 30))


        
        history_end = 50

        pygame.draw.line(
                        self,
                        (255, 255, 255),
                        (99, history_end-20),
                        (99, history_end)
                    )

        pygame.draw.line(
                        self,
                        (255, 255, 255),
                        (99, history_end),
                        (227, history_end)
                    )


        history = self.proximity_sensor.get_ir_history()
        values = list(history)
        prev_x = 0;
        prev_y = 0;
        actual_x = 100;
        actual_y = history_end;

        for i in range(len(values) - 1):
            value = values[i]
            value = value * 20

            prev_x = actual_x
            prev_y = actual_y

            actual_x = actual_x +2
            actual_y = int(50-value)

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

        self.draw_status()

    def  draw_status(self):
        
        status = self.proximity_sensor.get_status()
        if status == 1:
            status_string = "LED : on"
        else:
            status_string = "LED : off"

       

        text = self.font.render(
                status_string,
                True,
                (255, 255, 255)
            )

        self.blit(text, (10, 60))
        

        
        history_end = 80

        pygame.draw.line(
                        self,
                        (255, 255, 255),
                        (99, history_end-20),
                        (99, history_end)
                    )

        pygame.draw.line(
                        self,
                        (255, 255, 255),
                        (99, history_end),
                        (227, history_end)
                    )


        history = self.proximity_sensor.get_switch_history()
        values = list(history)
        prev_x = 0;
        prev_y = 0;
        actual_x = 100;
        actual_y = history_end;

        for i in range(len(values) - 1):
            value = values[i]
            value = value * 20

            prev_x = actual_x
            prev_y = actual_y

            actual_x = actual_x +2
            actual_y = int(80-value)

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



