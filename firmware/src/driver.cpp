#include "driver.h"

#include <rom/ets_sys.h>
#include <esp_log.h>

Driver::Driver(driver_pins_t* pins_, int device_count_) : 
    device_count(device_count_) 
{
    pins = *pins_;
    positions = new uint8_t[device_count];

    init_gpio();
    init_spi();
}

Driver::~Driver() {
    delete[] positions;
}

void Driver::set_position(int module_index, uint8_t pos) {
    if (module_index >= 0 && module_index < device_count /*&& positions[module_index] != pos*/) {
        positions[module_index] = pos;
        
        push_out();
    }
}

void Driver::demo() {
    gpio_set_level(pins.rclk, 0);
    gpio_set_level(pins.clr, 0);
    gpio_set_level(pins.oe, 0);

    while(true) {
        ESP_LOGI("demo", "RCLK HI");
        gpio_set_level(pins.rclk, 1);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        gpio_set_level(pins.rclk, 0);

        ESP_LOGI("demo", "CLR HI");
        gpio_set_level(pins.clr, 1);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        gpio_set_level(pins.clr, 0);

        ESP_LOGI("demo", "OE HI");
        gpio_set_level(pins.oe, 1);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        gpio_set_level(pins.oe, 0);
    }
}

void Driver::en() {
    gpio_set_level(pins.dcEnable, 1);
    gpio_set_level(pins.spinEnable, 1);
}

void Driver::dis() {
    gpio_set_level(pins.spinEnable, 0);
    gpio_set_level(pins.dcEnable, 0);
}

void Driver::init_spi() {
    esp_err_t ret;

    spi_bus_config_t bus_config;
    bus_config.miso_io_num = -1;
    bus_config.mosi_io_num = pins.ser;
    bus_config.sclk_io_num = pins.serclk;
    bus_config.quadwp_io_num = -1;
    bus_config.quadhd_io_num = -1;
    bus_config.max_transfer_sz = 0;
    bus_config.flags = SPICOMMON_BUSFLAG_MASTER;
    bus_config.isr_cpu_id = ESP_INTR_CPU_AFFINITY_AUTO;
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
    device_config.clock_speed_hz= 10000; // 10 KHz, no need to hurry
    device_config.input_delay_ns = 0;
    device_config.spics_io_num = -1;
    device_config.flags = 0;
    device_config.queue_size = 1;
    device_config.pre_cb = 0;
    device_config.post_cb = 0;

    ret = spi_bus_initialize(HSPI_HOST, &bus_config, 1);
    ESP_ERROR_CHECK(ret);
    ret = spi_bus_add_device(HSPI_HOST, &device_config, &spi);
    ESP_ERROR_CHECK(ret);
}

void Driver::init_gpio() {
    uint64_t mask = 0;
    mask |= (uint64_t) 0x01 << (uint64_t) pins.clr;
    mask |= (uint64_t) 0x01 << (uint64_t) pins.rclk;
    mask |= (uint64_t) 0x01 << (uint64_t) pins.oe;
    mask |= (uint64_t) 0x01 << (uint64_t) pins.spinEnable;
    mask |= (uint64_t) 0x01 << (uint64_t) pins.dcEnable;

    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = mask;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    // no spinning
    gpio_set_level(pins.dcEnable, 0);
    gpio_set_level(pins.spinEnable, 0);

    // enable register output
    gpio_set_level(pins.oe, 0);

    // RCLK and SRCLR on HIGH by default
    gpio_set_level(pins.clr, 1);
    gpio_set_level(pins.rclk, 1);

    // Zero all shift-registers
    clear_registers();

    // so all modules are on position 0
    for (int i = 0; i < device_count; i++) {
        positions[i] = 0;
    }
}

void Driver::clear_registers() {
    gpio_set_level(pins.clr, 0);

    gpio_set_level(pins.rclk, 0);
    ets_delay_us(1);
    gpio_set_level(pins.rclk, 1);

    gpio_set_level(pins.clr, 1);
}

void Driver::push_out() {

    spi_transaction_t tx;
    tx.flags = 0;
    tx.cmd = 0;
    tx.addr = 0;
    tx.length = 8 * device_count;
    tx.rxlength = 0;
    tx.rx_buffer = 0;
    tx.tx_buffer = positions;

    gpio_set_level(pins.rclk, 0);
    ESP_ERROR_CHECK(spi_device_transmit(spi, &tx));
    gpio_set_level(pins.rclk, 1);
}
