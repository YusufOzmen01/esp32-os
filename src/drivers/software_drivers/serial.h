#ifndef SERIAL_H
#define SERIAL_H
#include "drivers/drivers.h"

#include <Arduino.h>

enum SerialDriverAction {
    SERIAL_WRITE_CHAR,
    SERIAL_WRITE_TEXT,
    SERIAL_READ_CHAR
};

class SerialDriver : public DriverInterface {
public:
    SerialDriver(int baud_rate);

    uint8_t* write(int action_type, uint8_t* data) override;
    uint8_t* read(int action_type, uint8_t* data) override;

    // For serial, there is nothing to free (afaik)
    void update() override {};
    void free() override {};
};
#endif