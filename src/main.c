#include <avr/io.h>
#include <util/delay.h>

#include <dht11.h>

int main() {
    dht_data_pin_t dht_pin = (dht_data_pin_t){
        .bit = PB0,
        .ddr = &DDRB,
        .pin = &PINB,
        .port = &PORTB
    };
    dht11_init(&dht_pin);
    uint8_t temp = 0, humidity = 0;
    DDRD = 0xFF;
    PORTD = 0x00;
    while(1) {
        if(dht11_read() == 0) {
            temp = dht11_last_temp();
            humidity = dht11_last_humidity();
            PORTD = temp;
        }
        _delay_ms(2000);
        (void)humidity;
    }
    return 0;
}
