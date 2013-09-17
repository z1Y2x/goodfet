#!/usr/bin/env python
# GoodFET GPIO client

import sys, binascii, struct

# Standard verbs
READ  = 0x00
WRITE = 0x01
PEEK  = 0x02
POKE  = 0x03
SETUP = 0x10
START = 0x20
STOP  = 0x21
CALL  = 0x30
EXEC  = 0x31
NOK   = 0x7E
OK    = 0x7F

# GPIO commands
GPIO_WRITE_PIN              = 0x80
GPIO_READ_PIN               = 0x81
GPIO_ROUND_TRIP             = 0x82

from GoodFET import GoodFET
from intelhex import IntelHex

class GoodFETGPIO(GoodFET):

    """A GoodFET variant for manual GPIO."""

    GPIOAPP=0x60;
    APP=GPIOAPP;
 
    def _check_return(self, verb, length=0):
        if (self.app == self.APP) and \
           (self.verb == verb) and \
           (len(self.data) == length):
            print "OK"
            return True
        print "Failed!"
        return False

    def setup(self):
        """Move the FET into the GPIO configuration."""
        sys.stdout.write("Initializing GPIO...")
        self.writecmd(self.APP, SETUP)
        self._check_return(SETUP)

    def cfg_pin(self, pin, d):
        """Configure the GPIO pin."""
        sys.stdout.write("Configuring pin %d..." % pin)
        self.writecmd(self.APP, GPIO_CFG_PIN, 2, struct.pack("!BB", pin, d))
        self._check_return(GPIO_CFG_PIN)

    def write_pin(self, pin, v):
        """Set the GPIO pin to the given value."""
        sys.stdout.write("Setting pin %d to %d..." % (pin, v))
        self.writecmd(self.APP, GPIO_WRITE_PIN, 2, struct.pack("!BB", pin, v))
        self._check_return(GPIO_WRITE_PIN, 1)
        value = struct.unpack("!B", self.data)[0]
        return value

    def read_pin(self, pin):
        """Get the GPIO pin value."""
        sys.stdout.write("Reading pin %d..." % pin)
        self.writecmd(self.APP, GPIO_READ_PIN, 1, struct.pack("!B", pin))
        self._check_return(GPIO_READ_PIN, 1)
        value = struct.unpack("!B", self.data)[0]
        return value

    def round_trip(self, pout, v, pin):
        """Assert out pin and read value from in pin"""
        sys.stdout.write("Writing pin %d and reading pin %d..." % (pout, pin))
        self.writecmd(self.APP, GPIO_ROUND_TRIP, 3, struct.pack("!BBB", pout, v, pin))
        self._check_return(GPIO_ROUND_TRIP, 1)
        value = struct.unpack("!B", self.data)[0]
        return value

