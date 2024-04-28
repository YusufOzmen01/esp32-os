#include <vector>
#include "drivers.h"

Drivers::Drivers() {
    drivers = std::vector<DriverInterface*>();
}

void Drivers::add_driver(DriverInterface* driver) {
    for (auto d : drivers) {
        if (d->name == driver->name) {
            return;
        }
    }

    drivers.push_back(driver);
}

void Drivers::remove_driver(char* name) {
    for (int i = 0; i < drivers.size(); i++) {
        if (drivers[i]->name == name) {
            drivers[i]->free();
            drivers.erase(drivers.begin() + i);

            delete drivers[i];

            return;
        }
    }
}

void Drivers::update() {
    for (auto driver : drivers) {
        driver->update();
    }
}

DriverInterface* Drivers::get_driver_handle(char* name) {
    for (auto driver : drivers) {
        if (driver->name == name) {
            return driver;
        }
    }

    return nullptr;
}

std::vector<DriverInterface*>* Drivers::get_all_drivers() {
    return &drivers;
}