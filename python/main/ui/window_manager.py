from collections import deque




class WindowManager:
    def __init__(self):
        self.components = deque()


    def add(self, window):
        self.components.append(window)

    def draw(self, screen):
        for component in reversed(self.components):
            component.draw()
            screen.blit(component, (component.get_x_pos(), component.get_y_pos()))


    def to_front(self, window):
        pass


    def on_mouse_left(self, position):
        component = self.get_window_on_position(position)

        if component != None:
            print("found :")
            print(component.get_name())    
            component.on_mouse_left(position)


    def get_window_on_position(self, position):

        for component in self.components:
            if component.contains_point(position):
                return(component)

        return(None)
     

