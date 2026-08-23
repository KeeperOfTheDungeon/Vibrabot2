from ui.internal_window import InternalWindow
import pygame as pygame

class SensorView(InternalWindow):
    def __init__(self, x_pos, y_pos, width, height, name):
        super().__init__(x_pos ,y_pos, width, height, name)
        self.bkgColor = (40, 40, 40)
        self.font = pygame.font.Font(None, 24)
        self.component_list = list()
  
    def addComponent(self, ui_component):
        self.component_list.append(ui_component)


    def draw(self):
        super().draw()        
        pygame.draw.rect(self, (255, 255, 255), (0, 0, self.x_size, self.y_size), 2)
        pygame.draw.rect(self, (0, 0, 255), (2, 2, self.x_size-4, 26), 14)

        text = self.font.render(
            self.name,
            True,
            (255, 255, 255)
        )
    
        self.blit(text, (10, 8))


    def on_mouse_left(self, position):
        print("on mouse left sv")
        component, local_position  = self.get_component_on_position(position)

        if component != None:
            print(component.get_name())   
            local_x = local_position[0] - component.get_x_pos() 
            local_y = local_position[1] - component.get_y_pos()
            local = (local_x, local_y)
            component.on_mouse_left(local)


    def get_component_on_position(self, position):

        for component in self.component_list:
            x_pos = position[0] -self.x_pos 
            y_pos = position[1]- self.y_pos 
            local = (x_pos, y_pos)
            component_position = (local)
            if component.contains_point(component_position):
                return(component ,component_position )

        return(None,(0,0))
     


