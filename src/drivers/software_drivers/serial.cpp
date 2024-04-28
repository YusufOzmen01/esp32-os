#include "drivers/drivers.h"
#include "serial.h"
#include <Arduino.h>

SerialDriver::SerialDriver(int baud_rate) {
    name = "serial";
    locked = false;

    Serial.begin(baud_rate);
}

uint8_t* SerialDriver::write(int action_type, uint8_t* data) {
    switch (action_type) {
        case SERIAL_WRITE_CHAR:
            Serial.write(*(char*)data);
            break;
        case SERIAL_WRITE_TEXT:
            Serial.print((char*)data);
            break;
    }

    return nullptr;
}

uint8_t* SerialDriver::read(int action_type, uint8_t* data) {
    locked = true;

    switch (action_type) {
        case SERIAL_READ_CHAR:
            while (!Serial.available());

            return (uint8_t*) (new String((char) Serial.read()))->c_str();
    }

    locked = false;

    return nullptr;
}