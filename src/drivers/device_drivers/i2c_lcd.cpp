#include "i2c_lcd.h"

I2CLcdDriver::I2CLcdDriver(DeviceInterface* lcd) {
    name = "i2c_lcd";
    locked = false;

    this->lcd = lcd;

    delay(50);
    expanderWrite(LCD_BACKLIGHT);
    delay(1000);

    for (int i = 0; i < 3; i++) {
        write4bits(0x03 << 4);
        delay(5);
    };

    write4bits(0x02 << 4);

    send(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS, 0);
    send(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF, 0);
    send(LCD_CLEARDISPLAY, 0);
    delay(2);

	send(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT, 0);
    send(LCD_RETURNHOME, 0);
}

uint8_t* I2CLcdDriver::write(int action_type, uint8_t* data) {
    switch (action_type) {
        case I2C_LCD_WRITE_CHAR:
            write_char(*data);
            break;
        case I2C_LCD_WRITE_TEXT:
            write_string((char*) data);
            break;
        case I2C_LCD_SET_CURSOR:
            set_cursor(data[0], data[1]);
            break;
        case I2C_LCD_SHIFT_CURSOR_LEFT:
            send(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, 0);
            break;
        case I2C_LCD_SHIFT_CURSOR_RIGHT:
            send(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, 0);
            break;
        case I2C_LCD_CLEAR:
            clear();
            break;
    }

    return nullptr;
}

void I2CLcdDriver::write_char(char data) {
    send(data, Rs);
}

void I2CLcdDriver::write_string(char* data) {
    locked = true;
    for (int i = 0; i < strlen(data); i++) {
        write_char(data[i]);
    }
    locked = false;
}

void I2CLcdDriver::set_cursor(uint8_t col, uint8_t row) {
    int row_offsets[] = { 0x00, 0x40 };
    if (row > 1) row = 1;

    send(LCD_SETDDRAMADDR | (col + row_offsets[row]), 0);
}

void I2CLcdDriver::clear() {
    send(LCD_CLEARDISPLAY, 0);
    delay(2);
    send(LCD_RETURNHOME, 0);
    delay(2);
}

void I2CLcdDriver::send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
       write4bits((highnib)|mode);
	write4bits((lownib)|mode); 
}

void I2CLcdDriver::write4bits(uint8_t value) {
	expanderWrite(value);
	pulseEnable(value);
}

void I2CLcdDriver::expanderWrite(uint8_t _data){                                        
	lcd->write((int)(_data) | LCD_BACKLIGHT);
}

void I2CLcdDriver::pulseEnable(uint8_t _data){
	expanderWrite(_data | B00000100);
	delayMicroseconds(1);
	
	expanderWrite(_data & ~B00000100);
	delayMicroseconds(50);
} 