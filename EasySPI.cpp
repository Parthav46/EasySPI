#include "EasySPI.h"
#include <Arduino.h>
#include <SPI.h>

bufferNode** BufferQueue::usedList = (bufferNode**) malloc(10*sizeof(bufferNode**));
int BufferQueue::usedLen = 0;

void EasySPI::begin(SPIMode spiMode, uint8_t Select) {
    easySPISettings.SelctPin = Select;
    easySPISettings._spiMode = spiMode;
    if(spiMode == MASTER){
        pinMode(Select, OUTPUT);
        SPIClass::begin();
    } else {
        pinMode(Select, INPUT);
        digitalWrite(Select, HIGH);
        pinMode(SCK, INPUT);
        digitalWrite(SCK, LOW);
        pinMode(MOSI, INPUT);
        digitalWrite(MOSI, LOW);
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
    return easySPISettings.read_buffer.getLength();
}

char EasySPI::read() {
    if(easySPISettings._spiMode == MASTER) {
        return easySPISettings.read_buffer.remove();
    }
    char ans = easySPISettings.read_buffer.remove();
    return ans;
}

void EasySPI::write(char c) {
    if(easySPISettings._spiMode == MASTER) {
        digitalWrite(easySPISettings.SelctPin, LOW);
        easySPISettings.read_buffer.insert(SPIClass::transfer(c));
        digitalWrite(easySPISettings.SelctPin, HIGH);
    }
    easySPISettings.write_buffer.insert(c);
}

ISR(SPI_STC_vect) {
    if(easySPISettings._spiMode == SLAVE) {
        // Reading SPI Data Register
        easySPISettings.read_buffer.insert(SPDR);

        // Writing to SPI Data Register
        SPDR = easySPISettings.write_buffer.remove();
    }
}