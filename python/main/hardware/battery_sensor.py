from collections import deque

class BatterySensor:
    def __init__(self, name):
        self.capacity = 0.0
        self.name = name
    

    def get_name(self):
        return(self.name)

    def set_capacity(self, capacity):
        self.capacity = capacity 
        
        
    def get_capacity(self):
        return(self.capacity)


