#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>

#include "i2c.h"

I2CDevice::I2CDevice(String name, int address) {
    this->name = name;
    this->address = address;
}

void I2CDevice::write(uint8_t data) {
    Wire.beginTransmission(address);
    Wire.write(data);
    Wire.endTransmission();
}

uint8_t I2CDevice::read() {
    Wire.requestFrom(address, 1);
    return Wire.read();
}

I2CDiscoveryInterface::I2CDiscoveryInterface() {
    bus_type = "i2c";

    Wire.begin(23, 19);
}

std::vector<DeviceInterface*> I2CDiscoveryInterface::discover_devices() {
    std::vector<DeviceInterface*> devices = std::vector<DeviceInterface*>();

    for (int address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        uint8_t error = Wire.endTransmission();

        if (error == 0) {
            I2CDevice* device = new I2CDevice(*new String(("i2c_device[" + std::to_string(address) + "]").c_str()), address);

            devices.push_back(device);
        } else if (error != 2) {
            Serial.printf("Error %d at address 0x%02X\n", error, address);
        }
    }

    return devices;
}