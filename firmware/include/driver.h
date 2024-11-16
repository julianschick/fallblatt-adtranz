#ifndef DRIVER_H_
#define DRIVER_H_

#include <driver/gpio.h>
#include <driver/spi_master.h>

typedef struct {
    gpio_num_t ser;
    gpio_num_t serclk;
    //
    gpio_num_t clr;
    gpio_num_t rclk;
    gpio_num_t oe;
    //
    gpio_num_t spinEnable;
}  driver_pins_t;


class Driver {

public:
    Driver(driver_pins_t* pins_, int device_count);
    ~Driver();

    void set_position(int module_index, uint8_t pos);
    void demo();
    void en();
    void dis();

private:
    int device_count;
    uint8_t* positions;
    driver_pins_t pins;
    spi_device_handle_t spi;

    void init_spi();
    void init_gpio();

    void clear_registers();
    void push_out();

};


#endif