from collections import deque




class WindowManager:
    def __init__(self):
        self.elements = deque()


    def add(self, window):
        self.elements.append(window)

    def draw(self, screen):
        for element in reversed(self.elements):
            element.draw()
            screen.blit(element, (element.get_x_pos(), element.get_y_pos()))


    def to_front(self, window):
        pass


    def on_mouse_left(self, position):
        element = self.get_window_on_position(position)

        if element != None:
            print("found :")
            print(element.get_name())    
            element.on_mouse_left(position)


    def get_window_on_position(self, position):

        for element in self.elements:
            if element.contains_point(position):
                return(element)

        return(None)
     

