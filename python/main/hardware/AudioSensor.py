from collections import deque
from hardware.AudioSpectrum import AudioSpectrum



class AudioSensor:
    def __init__(self, name):
        self.name = name
        self.history = deque(maxlen=64)
        bins = AudioSpectrum()
        self.history.append(bins)


    def get_name(self):
        return(self.name)

    def add(self, bins):
        self.history.appendleft(bins)

    def get_last(self):
       return (self.history[0])

    def get_sensor_name(self, channel):
        return(self.get_name())


    def get_history(self):
        return(self.history)
