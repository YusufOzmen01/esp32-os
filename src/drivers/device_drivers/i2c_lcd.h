#ifndef I2C_LCD_H
#define I2C_LCD_H
#include "drivers/drivers.h"
#include "bus/bus.h"

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100  // Enable bit
#define Rw B00000010  // Read/Write bit
#define Rs B00000001  // Register select bit

enum I2CLcdDriverAction {
    I2C_LCD_WRITE_CHAR,
    I2C_LCD_WRITE_TEXT,
    I2C_LCD_SET_CURSOR,
    I2C_LCD_SHIFT_CURSOR_LEFT,
    I2C_LCD_SHIFT_CURSOR_RIGHT,
    I2C_LCD_CLEAR
};

class I2CLcdDriver : public DriverInterface {
    DeviceInterface* lcd;

    void send(uint8_t value, uint8_t mode);
    void write4bits(uint8_t value);
    void expanderWrite(uint8_t _data);
    void pulseEnable(uint8_t _data);

public:
    I2CLcdDriver(DeviceInterface* lcd);

    uint8_t* write(int action_type, uint8_t* data) override;
    uint8_t* read(int action_type, uint8_t* data) override {};

    void write_char(char data);
    void write_string(char* data);
    void set_cursor(uint8_t col, uint8_t row);
    void clear();

    void update() override {};
    void free() override {};
};

#endif