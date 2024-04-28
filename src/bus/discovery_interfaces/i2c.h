#ifndef I2C_H
#define I2C_H

#include "bus/bus.h"

class I2CDevice : public DeviceInterface {
public:
    int address;

    I2CDevice(String name, int address);

    void write(uint8_t data) override;
    uint8_t read() override;

    void free() override {};
};

class I2CDiscoveryInterface : public DeviceDiscoveryInterface {
public:
    I2CDiscoveryInterface();

    std::vector<DeviceInterface*> discover_devices() override;
};

#endif