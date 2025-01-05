#include "driver.h"

#include <rom/ets_sys.h>
#include <esp_log.h>
#include <esp_timer.h>

#define ESP_IDF 4
#define LOG_TAG "ABC_drv"

Driver::Driver(driver_pins_t* pins_, int device_count_) : 
    device_count(device_count_) 
{
    conv = new GrayConverter();
    pins = *pins_;
    positions = new uint8_t[device_count];

    init_gpio();
    init_spi();
}

Driver::~Driver() {
    delete[] positions;
    delete conv;
}

void Driver::set_position(int module_index, uint8_t pos) {
    if (module_index >= 0 && module_index < device_count) {
        positions[module_index] = pos;
    }
}

void Driver::update() {
    last_update = esp_timer_get_time();
    push_out();
    power_on();
    spin_on();
}

void Driver::loop() {
    if (spinning) {
        int64_t now = esp_timer_get_time();
        if (now - last_update > MAX_SPINNING_SECS * 1000000) {
            spin_off();
            power_off();
        }
    }
}

void Driver::init_spi() {
    // uint64_t mask = 0;
    // mask |= (uint64_t) 0x01 << (uint64_t) pins.ser;
    // mask |= (uint64_t) 0x01 << (uint64_t) pins.serclk;

    // gpio_config_t io_conf;
    // io_conf.intr_type = GPIO_INTR_DISABLE;
    // io_conf.mode = GPIO_MODE_OUTPUT;
    // io_conf.pin_bit_mask = mask;
    // io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    // io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    // ESP_ERROR_CHECK(gpio_config(&io_conf));

    // gpio_set_level(pins.ser, 0);
    // gpio_set_level(pins.serclk, 0);

    esp_err_t ret;

    spi_bus_config_t bus_config;
    bus_config.miso_io_num = -1;
    bus_config.mosi_io_num = pins.ser;
    bus_config.sclk_io_num = pins.serclk;
    bus_config.quadwp_io_num = -1;
    bus_config.quadhd_io_num = -1;
    bus_config.max_transfer_sz = 0;
    bus_config.flags = SPICOMMON_BUSFLAG_MASTER;
    #if ESP_IDF >= 5
    bus_config.isr_cpu_id = ESP_INTR_CPU_AFFINITY_AUTO;
    #endif 
    bus_config.intr_flags = 0;

    spi_device_interface_config_t device_config;
    device_config.command_bits = 0;
    device_config.address_bits = 0;
    device_config.dummy_bits = 0;
    device_config.mode = 0;
    device_config.clock_source = SPI_CLK_SRC_DEFAULT;
    device_config.duty_cycle_pos = 128; // 50%/50%
    device_config.cs_ena_pretrans = 0;
    device_config.cs_ena_posttrans = 0;
    device_config.clock_speed_hz= 1000; // 1 KHz, no need to hurry
    device_config.input_delay_ns = 0;
    device_config.spics_io_num = -1;
    device_config.flags = 0;
    device_config.queue_size = 1;
    device_config.pre_cb = 0;
    device_config.post_cb = 0;

    ret = spi_bus_initialize(HSPI_HOST, &bus_config, SPI_DMA_DISABLED);
    ESP_ERROR_CHECK(ret);
    ret = spi_bus_add_device(HSPI_HOST, &device_config, &spi);
    ESP_ERROR_CHECK(ret);
}

void Driver::init_gpio() {
    uint64_t mask = 0;
    mask |= (uint64_t) 0x01 << (uint64_t) pins.clr;
    mask |= (uint64_t) 0x01 << (uint64_t) pins.rclk;
    mask |= (uint64_t) 0x01 << (uint64_t) pins.oe;
    mask |= (uint64_t) 0x01 << (uint64_t) pins.spin_enable;
    mask |= (uint64_t) 0x01 << (uint64_t) pins.power;

    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = mask;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    // no spinning
    spin_off();
    
    // enable register output
    gpio_set_level(pins.oe, 0);

    // RCLK and SRCLR on HIGH by default
    gpio_set_level(pins.clr, 1);
    gpio_set_level(pins.rclk, 1);

    // initialize all positions to 0
    for (int i = 0; i < device_count; i++) {
        positions[i] = 0;
    }

}

// void Driver::clear_registers() {
//     gpio_set_level(pins.clr, 0);

//     gpio_set_level(pins.rclk, 0);
//     ets_delay_us(1);
//     gpio_set_level(pins.rclk, 1);

//     gpio_set_level(pins.clr, 1);
// }

void Driver::push_out() {

    uint8_t graycodes[device_count];
    for (int i = 0; i < device_count; i++) {
        graycodes[i] = conv->positionToGray(positions[i]);
    }

    spi_transaction_t tx;
    tx.flags = 0;
    tx.cmd = 0;
    tx.addr = 0;
    tx.length = 8 * device_count;
    tx.rxlength = 0;
    tx.rx_buffer = 0;
    tx.tx_buffer = &graycodes;

    gpio_set_level(pins.rclk, 0);
    ets_delay_us(1);
    ESP_ERROR_CHECK(spi_device_transmit(spi, &tx));
    ets_delay_us(1);
    gpio_set_level(pins.rclk, 1);

    //ESP_LOG_BUFFER_HEX(LOG_TAG, &graycodes, device_count);

    // BANG THE BIT
    // int us = 500;

    // for (int i = 0; i < device_count; i++) {
    //     for (int b = 0; b < 8; b++) {
    //         bool bit = (graycodes[i] << b) & 0x80;

    //         gpio_set_level(pins.ser, bit);
    //         ets_delay_us(us);
    //         gpio_set_level(pins.serclk, 1);
    //         ets_delay_us(us);
    //         gpio_set_level(pins.serclk, 0);
    //         ets_delay_us(us);
    //     }
    //     ets_delay_us(us*2);
    // }
    // gpio_set_level(pins.ser, 0);

    // gpio_set_level(pins.rclk, 0);
    // ets_delay_us(500);
    // gpio_set_level(pins.rclk, 1);

}
