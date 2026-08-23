class Robot:
    def __init__(self, name):
        self.name = name
        self.component_list = list()
      


    def process(self):
        pass

    def decode_com_package(package):
        pass

    def add_component(self, component):
        self.component_list.append(component)

    def get_component_on_name(self, name):
        for component in self.component_list:
            if component.get_name() == name:
                return (component)

        return (None)

