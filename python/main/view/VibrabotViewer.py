
from view.ProximitySensorView import ProximitySensorView
from view.RobotViewer import RobotViewer

import pygame as pygame
from view.ViewAudioSensor import ViewAudioSensor
from view.ViewColorSensor import ViewColorSensor
from view.view_light_sensor import view_light_sensor
from view.MotorView import MotorView

class VibrabotViewer(RobotViewer):
    def __init__(self, vibrabot):
        super().__init__(vibrabot)
        
        self.bulid_views()

        
    def add_view(self, component_name, view_class, x, y):
        component = self.robot.get_component_on_name(component_name)
        if component is None:
            return # make later an exception !

        view = view_class(x, y, component)
        self.window_manager.add(view)


    def bulid_views(self):

        self.light_sensor_left_view = self.add_view(
            "left eye",
            view_light_sensor,
            696,
            10
        )

        self.light_sensor_right_view = self.add_view(
            "right eye",
            view_light_sensor,
            848,
            10
        )

        self.color_sensor_view = self.add_view(
            "color Sensor",
            ViewColorSensor,
            696,
            478
        )

        self.audio_sensor_view = self.add_view(
            "Spectrum",
            ViewAudioSensor,
            696,
            650
        )

        self.proximity_sensor_left_view = self.add_view(
            "left proximity",
            ProximitySensorView,
            696,
            202
        )

        self.proximity_sensor_center_view = self.add_view(
            "center proximity",
            ProximitySensorView,
            696,
            294
        )

        self.proximity_sensor_right_view = self.add_view(
            "right proximity",
            ProximitySensorView,
            696,
            386
        )

        left_motor = self.robot.get_component_on_name("left motor")
        if left_motor is None:
            return # make later an exception !

        right_motor = self.robot.get_component_on_name("right motor")
        if left_motor is None:
            return # make later an exception !
        
        motor_view = MotorView(886, 478,left_motor, right_motor)
        self.window_manager.add(motor_view)




