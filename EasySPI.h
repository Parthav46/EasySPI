#ifndef EASYSPI_EASYSPI_H
#define EASYSPI_EASYSPI_H

#include <Arduino.h>
#include <SPI.h>

/** default pins for Arduino Uno. Pins defined in bootloader
 * MISO 12
 * MOSI 11
 * SCK  13
*/
enum SPIMode {MASTER, SLAVE};

class bufferNode {
public:
    char data;
    bufferNode* next;
};

class BufferQueue {
public:
    BufferQueue() {
        first = last = NULL;
        length = 0;
    }

    void insert(char data) {
        bufferNode* current;
        if(usedLen > 0){
            current = (bufferNode*)*(usedList + usedLen);
            usedLen--;
        }
        else current = new bufferNode();
        current->data = data;
        current->next = NULL;
        if(length > 0) {
            last->next = current;
            last = current;
        } else {
            first = last = current;
        }
        length++;
    }

    char remove() {
        char data = 0x00;
        if(length > 0) {
            data = first->data;
            if(length > 1) {
                usedLen++;
                *(usedList + usedLen) = first;
                first = first->next;
            }
            length--;
        }
        return data;
    }

    int getLength() {
        return length;
    }

private:
    static bufferNode **usedList;
    static int usedLen ;
    bufferNode *first{}, *last{};
    int length;
};

struct EasySPISettings {
    SPIMode _spiMode = MASTER;
    BufferQueue read_buffer;
    BufferQueue write_buffer;
    uint8_t SelctPin = 0x00;
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