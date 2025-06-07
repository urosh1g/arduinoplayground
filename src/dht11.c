#include <avr/io.h>
#include <util/delay.h>

#include <dht11.h>

static volatile dht_data_pin_t data_pin = {0};
static volatile uint8_t last_temp = 0;
static volatile uint8_t last_humidity = 0;

void dht11_init(dht_data_pin_t* pin) {
    data_pin = (dht_data_pin_t){
        .bit = pin->bit,
        .ddr = pin->ddr,
        .port = pin->port,
        .pin = pin->pin
    };
    *data_pin.ddr |= _BV(data_pin.bit);
    *data_pin.port |= _BV(data_pin.bit);
}

static inline uint8_t read_byte() {
    uint8_t byte = 0;
    for(uint16_t i = 0; i < 8; i++) {
       while(!(*data_pin.pin & _BV(data_pin.bit)))
           ;
       _delay_us(40);
       byte <<= 1;
       if(*data_pin.pin & _BV(data_pin.bit)) {
           byte |= 1;
       }
       while((*data_pin.pin & _BV(data_pin.bit)))
           ;
    }
    return byte;
}

uint8_t dht11_read() {
    *data_pin.port &= ~_BV(data_pin.bit);
    _delay_ms(20);

    *data_pin.port |= _BV(data_pin.bit);
    *data_pin.ddr &= ~_BV(data_pin.bit);
    _delay_us(40);

    //DHT keeps low for 80us
    uint16_t c = 0;
    uint32_t max_cycles = 1280;
    while(!(*data_pin.pin & _BV(data_pin.bit))) {
        if(c++ >= max_cycles) {
            return 3;
        }
    }
    c = 0;
    //DHT pulls high for 80us
    while(*data_pin.pin & _BV(data_pin.bit)) {
        if(c++ >= max_cycles) {
            return 3;
        }
    }

    asm("cli");
    last_humidity = read_byte();
    read_byte(); // HUMIDITY FRACTION
    last_temp = read_byte();
    read_byte(); // TEMP FRACTION
    read_byte(); // CRC
    asm("sei");
    return 0;
}

uint8_t dht11_last_temp() {
    return last_temp;
}

uint8_t dht11_last_humidity() {
    return last_humidity;
}

