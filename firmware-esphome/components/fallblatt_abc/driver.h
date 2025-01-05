#ifndef DRIVER_H_
#define DRIVER_H_

#include <driver/gpio.h>
#include <driver/spi_master.h>

#include "gray.h"

// takes around 8 secs to spin thru, so give it 3 chances to find its position
#define MAX_SPINNING_SECS 26

typedef struct {
    gpio_num_t ser;
    gpio_num_t serclk;
    //
    gpio_num_t clr;
    gpio_num_t rclk;
    gpio_num_t oe;
    //
    gpio_num_t spin_enable;
    gpio_num_t power;
}  driver_pins_t;

class Driver {

public:
    Driver(driver_pins_t* pins_, int device_count);
    ~Driver();

    void set_position(int module_index, uint8_t pos);
    int get_device_count() { return device_count; };

    void update();
    void loop();
private:
    int device_count;
    uint8_t* positions;
    
    GrayConverter* conv;
    driver_pins_t pins;
    spi_device_handle_t spi;

    int64_t last_update;

    void init_spi();
    void init_gpio();

    void power_on() { gpio_set_level(pins.power, 1); }
    void power_off() { gpio_set_level(pins.power, 0); }

    bool spinning = false;
    void spin_on() { gpio_set_level(pins.spin_enable, 1); spinning = true; }
    void spin_off() { gpio_set_level(pins.spin_enable, 0); spinning = false; }

    //void clear_registers();
    void push_out();

};


#endif