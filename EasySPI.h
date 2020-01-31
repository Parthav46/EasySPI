#ifndef EASYSPI_EASYSPI_H
#define EASYSPI_EASYSPI_H

#include <stdint-gcc.h>
#include "Arduino.h"
#include "avr/interrupt.h"
#include "avr/io.h"
#include "SPI/src/SPI.h"

/** default pins for Arduino Uno. Pins defined in bootloader
 * MISO 12
 * MOSI 11
 * SCK  13
*/
enum SPIMode {MASTER, SLAVE};

class EasySPISettings {
public:
    static SPIMode _spiMode;
    static int read_available;
    static int write_available;
    static char* read_buffer;
    static char* write_buffer;
    static uint8_t SelctPin;
};

class EasySPI {
public:
    static void begin(SPIMode spiMode, uint8_t Select);
    static int available();
    static char read();
    static void write(char c);
};

#endif //EASYSPI_EASYSPI_H