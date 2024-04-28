#include <Arduino.h>
#include "bus/bus.h"

#include "bus/discovery_interfaces/i2c.h"

#include "drivers/drivers.h"
#include "drivers/software_drivers/serial.h"
#include "drivers/device_drivers/i2c_lcd.h"

Bus* bus;
Drivers* drivers;

char* NEW_LINE = "\n";

void setup(){
    Serial.begin(9600);

    bus = new Bus();
    drivers = new Drivers();

    SerialDriver* serial_driver = new SerialDriver(9600);

    I2CDiscoveryInterface* i2c_discovery_interface = new I2CDiscoveryInterface();
    bus->add_discovery_interface(i2c_discovery_interface);

    serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) "\nStarting up ESP-OS\n");
    
    serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) "Bus discovery interfaces:\n");
    std::vector<DeviceDiscoveryInterface*>* all_discovery_interfaces = bus->get_all_discovery_interfaces();
    for (auto discovery_interface : *all_discovery_interfaces) {
        serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) discovery_interface->bus_type.c_str());
        serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) "\n");
    }

    serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) "\nDiscovering devices on bus...");
    bus->discover_devices();

    std::vector<DeviceInterface*>* all_devices = bus->get_all_devices();
    serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) ("\nDevices [" + std::to_string(all_devices->size()) + "]:\n").c_str());

    for (auto device : *all_devices) {
        serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) (strlen(device->name.c_str()) == 0 ? "Unnamed device" : device->name).c_str());
        serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) "\n");
    }

    drivers->add_driver(serial_driver);
    drivers->add_driver(serial_driver);

    DeviceInterface* lcd_device = bus->get_device_handle("i2c_device[39]");
    if (lcd_device == nullptr) {
        serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) "Could not find LCD device!\n");
    } else {
        I2CLcdDriver* lcd_driver = new I2CLcdDriver(lcd_device);
        drivers->add_driver(lcd_driver);
    }

    serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) "\nDrivers:\n");

    std::vector<DriverInterface*>* all_drivers = drivers->get_all_drivers();
    for (auto driver : *all_drivers) {
        serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) driver->name);
        serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) "\n");
    }

    serial_driver->write(SERIAL_WRITE_TEXT, (uint8_t*) "\nStarting main OS\n\n");
}

void loop(){
    drivers->update();

    DriverInterface* serial = drivers->get_driver_handle("serial");
    if (serial == nullptr) {
        return;
    }

    DriverInterface* lcd = drivers->get_driver_handle("i2c_lcd");
    if (lcd == nullptr) {
        return;
    }

    char* data = (char*) serial->read(SERIAL_READ_CHAR, nullptr);

    switch ((int) *data) {
        case 8:
            lcd->write(I2C_LCD_SHIFT_CURSOR_LEFT, nullptr);
            lcd->write(I2C_LCD_WRITE_CHAR, (uint8_t*) " ");
            lcd->write(I2C_LCD_SHIFT_CURSOR_LEFT, nullptr); 
            break;
        case 27:
            lcd->write(I2C_LCD_CLEAR, nullptr);
            break;
        case 67:
            lcd->write(I2C_LCD_SHIFT_CURSOR_RIGHT, nullptr);
            break;
        case 68:
            lcd->write(I2C_LCD_SHIFT_CURSOR_LEFT, nullptr);
            break;
        default:
            lcd->write(I2C_LCD_WRITE_CHAR, (uint8_t*) data);
            break;
    }
}