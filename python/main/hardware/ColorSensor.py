from collections import deque

class ColorSensor:
    def __init__(self, name):
        self.intensity = 0.0
        self.name = name
        self.history = deque(maxlen=64)

    def get_name(self):
        return(self.name)

    def set_intensity(self, intensity):
        self.intensity = intensity 
        self.history.append(intensity) 
        
    def get_intensity(self):
        return(self.intensity)

    def get_history(self):
        return(self.history)


