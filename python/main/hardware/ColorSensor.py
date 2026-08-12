from collections import deque

from hardware.light_sensor import Light_sensor

sensor_names = [
    "clear",
    "red",
    "green",
    "blue",
    "ir"
]


class ColorSensor:
    def __init__(self, name):
        self.intensity = 0.0
        self.name = name
        self.light_sensors = [
            Light_sensor(sensor_name)
            for sensor_name in sensor_names]


    def get_name(self):
        return(self.name)

    def set_intensity(self, channel, intensity):
        if channel > len (self.light_sensors):
            return (0);
        self.light_sensors[channel].set_intensity(intensity)
    
        
    def get_intensity(self, channel):
        if channel > len (self.light_sensors):
            return (0);
    
        return(self.light_sensors[channel].get_intensity())

    def get_sensor_name(self, channel):
        if channel > len (self.light_sensors):
            return ("default");
    
        return(self.light_sensors[channel].get_name())


    def get_history(self):
        return(self.history)


