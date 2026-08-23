import queue

from hardware.light_sensor import Light_sensor
from hardware.AudioSensor import AudioSensor
from hardware.ProximitySensor import ProximitySensor
from hardware.ColorSensor import ColorSensor
from hardware.AudioSpectrum import AudioSpectrum
from hardware.motor import Motor
from hardware.Robot import Robot


class Vibrabot(Robot):
    def __init__(self, name):
        super().__init__(name)
        self.light_sensor_left = Light_sensor("left eye")
        self.add_component(self.light_sensor_left)

        self.light_sensor_right = Light_sensor("right eye")
        self.add_component(self.light_sensor_right)

        self.color_sensor = ColorSensor("color Sensor") 
        self.add_component(self.color_sensor)

        self.audio_sensor = AudioSensor("Spectrum")
        self.add_component(self.audio_sensor)


        self.proximity_sensor_left = ProximitySensor("left proximity")
        self.add_component(self.proximity_sensor_left)
        
        self.proximity_sensor_center = ProximitySensor("center proximity")
        self.add_component(self.proximity_sensor_center)

        self.proximity_sensor_right = ProximitySensor("right proximity")
        self.add_component(self.proximity_sensor_right)

        self.motor_left = Motor("left motor")
        self.add_component(self.motor_left)

        self.motor_right = Motor("left motor")
        self.add_component(self.motor_right)

        self.rx_queue = queue.Queue()
        self.tx_queue = queue.Queue()

      #  ble = Ble(rx_queue,tx_queue)


    def process(self):
        super().process(self)

        if not self.rx_queue.empty():
            data = self.rx_queue.get()
            self.decode_ble_package(data)

        


    def decode_com_package(self, package):
        super().decode_com_package(package)
        value = int.from_bytes(package[0:1], byteorder='little')
    
        match  value:
            case  0xa0:
                self.decode_ble_light_Package(package)
            case 0xa1:
                self.decode_ble_proximity_Package(package)
            case 0xB0:
                self.decode_ble_fft_Package(package)




    def decode_ble_fft_Package(self,package):
        spectrum = AudioSpectrum()
        position = 2
        
        for index in  range(3):
            bin = int.from_bytes(package[position :position +2], byteorder='little')

            level = int.from_bytes(package[position+2 :position +4], byteorder='little')
            spectrum.set_bin(index, bin,level)
            position = position + 4

            self.audio_sensor.add(spectrum)



    def decode_ble_light_Package(self, package):
        value = int.from_bytes(package[2:4], byteorder='little')
        f = float(value)/4906
        self.light_sensor_left.set_intensity(f )  
        
        value = int.from_bytes(package[4:6], byteorder='little')
        f = float(value)/4906
        self.light_sensor_right.set_intensity(f )  

        value = int.from_bytes(package[6:8], byteorder='little')
        f = float(value)/65536
        self.color_sensor.set_intensity(0,f)  

        value = int.from_bytes(package[8:10], byteorder='little')
        f = float(value)/65536
        f = f * (34.0 / 41.0)
        self.color_sensor.set_intensity(1,f)  

        value = int.from_bytes(package[10:12], byteorder='little')
        f = float(value)/65536
        f = f * (34.0 / 39.0)
        self.color_sensor.set_intensity(2,f)  

        value = int.from_bytes(package[12:14], byteorder='little')
        f = float(value)/65536
        self.color_sensor.set_intensity(3,f)  

        value = int.from_bytes(package[14:16], byteorder='little')
        f = float(value)/65536
        self.color_sensor.set_intensity(4,f)  



    def decode_ble_proximity_Package(self, package):

        position = 2

        value  = int.from_bytes(package[position :position +2], byteorder='little')
        f = float(value)/4906
        self.left_proximity_sensor.set_intensity(f)

        position += 2
        value  = int.from_bytes(package[position :position +2], byteorder='little')
        f = float(value)/4906
        self.center_proximity_sensor.set_intensity(f)

        position += 2
        value  = int.from_bytes(package[position :position +2], byteorder='little')
        f = float(value)/4906
        self.right_proximity_sensor.set_intensity(f)

        position += 2
        value  = int.from_bytes(package[position :position +2], byteorder='little')
        f = float(value)
        
        self.left_proximity_sensor.set_status(f)
        self.center_proximity_sensor.set_status(f)
        self.right_proximity_sensor.set_status(f)
        
    