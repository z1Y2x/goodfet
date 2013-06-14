# USBConfiguration.py
#
# Contains class definition for USBConfiguration.

class USBConfiguration:
    def __init__(self, configuration_index, configuration_string, interfaces):
        self.configuration_index        = configuration_index
        self.configuration_string       = configuration_string
        self.configuration_string_index = 0
        self.interfaces                 = interfaces

    def set_configuration_string_index(self, i):
        self.configuration_string_index = i

    def get_descriptor(self):
        interface_descriptors = bytearray()
        for i in self.interfaces:
            interface_descriptors += i.get_descriptor()

        total_len = len(interface_descriptors) + 7

        d = bytes([
                7,          # length of descriptor in bytes
                2,          # descriptor type 2 == configuration
                total_len & 0xff,
                (total_len >> 8) & 0xff,
                len(self.interfaces),
                self.configuration_index,
                self.configuration_string_index
        ])

        return d + interface_descriptors

