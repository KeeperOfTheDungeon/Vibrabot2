import queue

from ble.ble import Ble
from hardware.battery_sensor import BatterySensor
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

        self.battery_sensor = BatterySensor("battery_sensor")
        self.add_component(self.battery_sensor)


        self.motor_right = Motor("right motor")
        self.add_component(self.motor_right)

        self.rx_queue = queue.Queue()
        self.tx_queue = queue.Queue()

        ble = Ble(self.rx_queue,self.tx_queue)


    def process(self):
        #super().process(self)

        while True:
            try:
                data = self.rx_queue.get_nowait()
            except queue.Empty:
                break

            self.decode_com_package(data)
 #           print("process")

  #      print("*********************process end")



    def decode_com_package(self, package):
        #super().decode_com_package(package)
        value = int.from_bytes(package[0:1], byteorder='little')
    
        match  value:
            case  0xa0:
                self.decode_ble_light_Package(package)
                self.send_motor_data()
            case 0xA2:
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
        self.decode_ble_light_data(package)
        self.decode_ble_color_data(package)
        self.decode_ble_proximity_data(package)


    def decode_ble_light_data(self, package):
        value = int.from_bytes(package[2:4], byteorder='little')
        f = float(value)/4906
        self.light_sensor_left.set_intensity(f )  
        
        value = int.from_bytes(package[4:6], byteorder='little')
        f = float(value)/4906
        self.light_sensor_right.set_intensity(f )  



    def decode_ble_color_data(self, package):
        value = int.from_bytes(package[6:8], byteorder='little')
        f = float(value)/65536
        self.color_sensor.set_intensity(0,f)  

        value = int.from_bytes(package[8:10], byteorder='little')
        f = float(value)/65536
        #f = f * (34.0 / 41.0)
        f = f * 2.1
        self.color_sensor.set_intensity(1,f)  

        value = int.from_bytes(package[10:12], byteorder='little')
        f = float(value)/65536
       # f = f * (34.0 / 39.0)
        self.color_sensor.set_intensity(2,f)  

        value = int.from_bytes(package[12:14], byteorder='little')
        f = float(value)/65536
        f = f * 1.5
        self.color_sensor.set_intensity(3,f)  

        value = int.from_bytes(package[14:16], byteorder='little')
        f = float(value)/65536
        self.color_sensor.set_intensity(4,f)  



    def decode_ble_proximity_data(self, package):
        value  = int.from_bytes(package[16:18], byteorder='little')
        f = float(value)/4906
        self.proximity_sensor_left.set_intensity(f)


        value  = int.from_bytes(package[20:22], byteorder='little')
        f = float(value)/4906
        self.proximity_sensor_center.set_intensity(f)

        value  = int.from_bytes(package[24:26], byteorder='little')
        f = float(value)/4906
        self.proximity_sensor_right.set_intensity(f)
        
        self.proximity_sensor_left.set_status(False)
        self.proximity_sensor_center.set_status(False)
        self.proximity_sensor_right.set_status(False)


        value  = int.from_bytes(package[18:20], byteorder='little')
        f = float(value)/4906
        self.proximity_sensor_left.set_intensity(f)


        value  = int.from_bytes(package[22:24], byteorder='little')
        f = float(value)/4906
        self.proximity_sensor_center.set_intensity(f)

        value  = int.from_bytes(package[26:28], byteorder='little')
        f = float(value)/4906
        self.proximity_sensor_right.set_intensity(f)

       
        
        self.proximity_sensor_left.set_status(True)
        self.proximity_sensor_center.set_status(True)
        self.proximity_sensor_right.set_status(True)


        value = int.from_bytes(package[28:30], byteorder='little')
        f = float(value)/4095
        print(f)
        self.battery_sensor.set_capacity(f)  
 
    

    def send_motor_data(self):

      
        data_packet = bytearray([0xB0,0, 0])
      

        left_motor_value = self.motor_left.get_control_value()
        right_motor_value = self.motor_right.get_control_value()

        data_packet[1] = int(left_motor_value *255)
        data_packet[2] = int(right_motor_value *255)

        self.tx_queue.put_nowait(data_packet)

        print("Motor data send")
        print("Queue size:", self.tx_queue.qsize())  