#ifndef DRIVERS_H
#define DRIVERS_H
#include <cstdint>
#include "bus/bus.h"

class DriverInterface {
public:
    char* name;
    DeviceInterface* device;

    bool locked;

    virtual uint8_t* write(int action_type, uint8_t* data);
    virtual uint8_t* read(int action_type, uint8_t* data);
    virtual void update() = 0;
    virtual void free() = 0;
};

class Drivers {
    std::vector<DriverInterface*> drivers;

public:
    Drivers();

    void add_driver(DriverInterface* driver);
    DriverInterface* get_driver_handle(char* name);
    std::vector<DriverInterface*>* get_all_drivers();

    void remove_driver(char* name);

    void update();
};

#endif