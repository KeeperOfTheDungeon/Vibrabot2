from collections import deque

class Switch:
    def __init__(self, name):
        self.status = 0.0
        self.name = name
        self.history = deque(maxlen=64)

    def get_name(self):
        return(self.name)

    def set_status(self, status):
        self.status  = status 
        self.history.append(status) 
        
    def get_status(self):
        return(self.status)

    def get_history(self):
        return(self.history)


