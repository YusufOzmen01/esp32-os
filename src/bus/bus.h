#ifndef BUS_H
#define BUS_H
#include <cstdint>
#include <vector>
#include <Arduino.h>

class DeviceInterface {
public:
    String name;
    String interface_type;

    virtual void write(uint8_t data) = 0;
    virtual uint8_t read() = 0;
    virtual void free() = 0;
};

class DeviceDiscoveryInterface {
public:
    String bus_type;

    virtual std::vector<DeviceInterface*> discover_devices() = 0;
};

class Bus {
private:
    std::vector<DeviceInterface*> devices;
    std::vector<DeviceDiscoveryInterface*> discovery_interfaces;

public:
    Bus();

    void add_device(DeviceInterface* device);
    void remove_device(String name);
    void add_discovery_interface(DeviceDiscoveryInterface* discovery_interface);
    void discover_devices();

    DeviceInterface* get_device_handle(String name);
    std::vector<DeviceInterface*>* get_all_devices();
    std::vector<DeviceDiscoveryInterface*>* get_all_discovery_interfaces();
};

#endif