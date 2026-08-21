from collections import deque

from hardware.light_sensor import Light_sensor
from hardware.switch import Switch



class ProximitySensor:
    def __init__(self, sensor_name):
        self.intensity = 0.0
        self.name = sensor_name
        self.light_sensor = Light_sensor(sensor_name)
        self.switch = Switch(sensor_name)



    def get_name(self):
        return(self.name)

    def set_intensity(self, intensity):
        self.light_sensor.set_intensity(intensity)
    
        
    def get_intensity(self):
        return(self.light_sensor.get_intensity())

    def set_status(self, status):
        self.switch.set_status(status)

    def get_status(self):
        return (self.switch.get_status())


    def get_sensor_name(self):
        return(self.light_sensor.get_name())


    def get_ir_history(self):
        return(self.light_sensor.get_history())

    def get_switch_history(self):
        return(self.switch.get_history())
