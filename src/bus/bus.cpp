#include <vector>
#include <Arduino.h>
#include "bus.h"

Bus::Bus() {
    devices = std::vector<DeviceInterface*>();
    discovery_interfaces = std::vector<DeviceDiscoveryInterface*>();
}

void Bus::add_device(DeviceInterface* device) {
    for (auto device : devices) {
        if (device->name == device->name) {
            return;
        }
    }

    devices.push_back(device);
}

void Bus::remove_device(String name) {
    for (int i = 0; i < devices.size(); i++) {
        if (devices[i]->name == name) {
            devices[i]->free();
            devices.erase(devices.begin() + i);

            delete devices[i];

            return;
        }
    }
}

void Bus::add_discovery_interface(DeviceDiscoveryInterface* discovery_interface) {
    for (auto device_discovery_interface : discovery_interfaces) {
        if (device_discovery_interface->bus_type == discovery_interface->bus_type) {
            return;
        }
    }

    discovery_interfaces.push_back(discovery_interface);
}

void Bus::discover_devices() {
    for (auto device : devices) {
        delete device;
    }

    for (auto discovery_interface : discovery_interfaces) {
        auto discovered_devices = discovery_interface->discover_devices();
        
        for (auto device : discovered_devices) {
            add_device(device);
        }
    }
}

DeviceInterface* Bus::get_device_handle(String name) {
    for (auto device : devices) {
        if (device->name == name) {
            return device;
        }
    }

    return nullptr;
}

std::vector<DeviceInterface*>* Bus::get_all_devices() {
    return &devices;
}

std::vector<DeviceDiscoveryInterface*>* Bus::get_all_discovery_interfaces() {
    return &discovery_interfaces;
}