#ifndef _dht11_h
#define _dht11_h

#include <stdint.h>

typedef struct {
    volatile uint8_t* ddr;
    volatile uint8_t* port;
    volatile uint8_t* pin;
    uint8_t  bit;
} dht_data_pin_t;

void dht11_init(dht_data_pin_t* pin);
uint8_t dht11_read();
uint8_t dht11_last_temp();
uint8_t dht11_last_humidity();

#endif

