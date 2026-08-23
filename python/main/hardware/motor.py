from collections import deque

class Motor:
    def __init__(self, name):
        self.value = 0.0
        self.control_value = 0.0
        self.dirty = False
        self.name = name
        self.history = deque(maxlen=64)
        

    def get_name(self):
        return(self.name)
    
    def get_history(self):
        return(self.history)

    def set_value(self, value):
        self.value = value

    def get_value(self, value):
        return(self.value)

    def set_control_value(self, value):
            self.control_value = value
            self.dirty = False

    def get_control_value(self):
            return(self.control_value)

    def is_dirty(self):
            return (self.dirty)
