#include <esp_log.h>

#include "driver.h"

extern "C" void app_main() {

    driver_pins_t pins;
    pins.ser = GPIO_NUM_13;
    pins.serclk = GPIO_NUM_14;
    pins.oe = GPIO_NUM_15;
    pins.clr = GPIO_NUM_19;
    pins.rclk = GPIO_NUM_18;
    pins.spinEnable = GPIO_NUM_4;

    Driver* drv = new Driver(&pins, 1);
    
    while (true) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        drv->set_position(0, 3);
        ESP_LOGI("MAIN", "Pushing out.");
    }


}