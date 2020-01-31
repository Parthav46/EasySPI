#include "EasySPI.h"

void EasySPI::begin(SPIMode spiMode, uint8_t Select) {
    EasySPISettings::SelctPin = Select;
    EasySPISettings::_spiMode = spiMode;
    if(spiMode == MASTER){
        pinMode(Select, OUTPUT);
        SPIClass::begin();
    } else {
        pinMode(Select, INPUT);
        pinMode(SCK, INPUT);
        pinMode(MOSI, INPUT);
        pinMode(MISO, OUTPUT);

        SPCR |= _BV(SPE);
        SPIClass::attachInterrupt();
    }
}

int EasySPI::available() {
    if(EasySPISettings::_spiMode == MASTER) {
        return 0;
    }
    return EasySPISettings::read_available;
}

char EasySPI::read() {
    if(EasySPISettings::_spiMode == MASTER) {
        return *(EasySPISettings::read_buffer);
    }
    char ans = *(EasySPISettings::read_buffer);
    EasySPISettings::read_buffer = EasySPISettings::read_buffer+1;
    EasySPISettings::read_available --;
    return ans;
}

void EasySPI::write(char c) {
    if(EasySPISettings::_spiMode == MASTER) {
        *(EasySPISettings::read_buffer) = SPIClass::transfer(c);
    }
    *(EasySPISettings::write_buffer + EasySPISettings::write_available++) = c;
}

ISR(SPI_STC_vect) {
    if(EasySPISettings::_spiMode == SLAVE && digitalRead(EasySPISettings::SelctPin)) {
        // Reading SPI Data Register
        *(EasySPISettings::read_buffer + EasySPISettings::read_available++) = SPDR;

        // Writing to SPI Data Register
        if(EasySPISettings::write_available){
            SPDR = *(EasySPISettings::write_buffer);
            EasySPISettings::write_buffer = EasySPISettings::write_buffer + 1;
            EasySPISettings::write_available--;
        }
    }
}