#include "EasySPI.h"
#include <Arduino.h>
#include "avr/interrupt.h"
#include <SPI.h>

void EasySPI::begin(SPIMode spiMode, uint8_t Select) {
    easySPISettings.read_available = 0;
    easySPISettings.write_available = 0;
    easySPISettings.read_buffer = (char*) malloc(64 * sizeof(char));
    easySPISettings.write_buffer = (char*) malloc(64 * sizeof(char));
    easySPISettings.SelctPin = Select;
    easySPISettings._spiMode = spiMode;
    if(spiMode == MASTER){
        pinMode(Select, OUTPUT);
        SPIClass::begin();
    } else {
        pinMode(Select, INPUT);
        digitalWrite(Select, LOW);
        pinMode(SCK, INPUT);
        pinMode(MOSI, INPUT);
        pinMode(MISO, OUTPUT);

        SPCR |= _BV(SPE);
        SPIClass::attachInterrupt();
    }
}

void EasySPI::end() {
    if(easySPISettings._spiMode == MASTER) SPI.end();
    else SPI.detachInterrupt();
}

int EasySPI::available() {
    if(easySPISettings._spiMode == MASTER) {
        return 0;
    }
    return easySPISettings.read_available;
}

char EasySPI::read() {
    if(easySPISettings._spiMode == MASTER) {
        return *(easySPISettings.read_buffer);
    }
    char ans = *(easySPISettings.read_buffer);
    easySPISettings.read_buffer = easySPISettings.read_buffer+1;
    easySPISettings.read_available --;
    return ans;
}

void EasySPI::write(char c) {
    if(easySPISettings._spiMode == MASTER) {
        digitalWrite(easySPISettings.SelctPin, HIGH);
        *(easySPISettings.read_buffer) = SPIClass::transfer(c);
        digitalWrite(easySPISettings.SelctPin, LOW);
    }
    *(easySPISettings.write_buffer + easySPISettings.write_available++) = c;
}

ISR(SPI_STC_vect) {
    if(easySPISettings._spiMode == SLAVE) {
        // Reading SPI Data Register
        *(easySPISettings.read_buffer + easySPISettings.read_available++) = SPDR;

        // Writing to SPI Data Register
        if(easySPISettings.write_available){
            SPDR = *(easySPISettings.write_buffer);
            easySPISettings.write_buffer = easySPISettings.write_buffer + 1;
            easySPISettings.write_available--;
        } else {
            SPDR = 0x00;
        }
    }
}