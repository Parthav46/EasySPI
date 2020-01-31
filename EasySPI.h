#ifndef EASYSPI_EASYSPI_H
#define EASYSPI_EASYSPI_H

#include <Arduino.h>
#include "avr/interrupt.h"
#include <SPI.h>

/** default pins for Arduino Uno. Pins defined in bootloader
 * MISO 12
 * MOSI 11
 * SCK  13
*/
enum SPIMode {MASTER, SLAVE};

struct EasySPISettings {
    SPIMode _spiMode;
    int read_available;
    int write_available;
    char* read_buffer;
    char* write_buffer;
    uint8_t SelctPin;
};

static EasySPISettings easySPISettings;

class EasySPI {
public:
    static void begin(SPIMode spiMode, uint8_t Select);
    static void end();
    static int available();
    static char read();
    static void write(char c);
};

extern EasySPI EasySPI;

#endif //EASYSPI_EASYSPI_H