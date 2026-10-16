from collections import deque

class TemperatureSensor:
    def __init__(self, name):
        self.temperature = 0.0
        self.name = name
    

    def get_name(self):
        return(self.name)

    def set_temperature(self, temperature):
        self.temperature = temperature 
        
        
    def get_temperature(self):
        return(self.temperature)


