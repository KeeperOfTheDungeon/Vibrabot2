from collections import deque

class Motor:
    def __init__(self, name):
        self.value = 0.0
        self.name = name
        self.history = deque(maxlen=64)

    def get_name(self):
        return(self.name)
    
    def get_history(self):
        return(self.history)


