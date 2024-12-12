#include "driver.h"

#include <esp_log.h>
#include <rom/ets_sys.h>
#include <rom/uart.h>

#include <string>
#include <stdexcept>

#include "gray.h"

void main1();
void main2();

extern "C" void app_main() {
    main1();
}

void main2() {
    driver_pins_t pins;
    pins.ser = GPIO_NUM_13;
    pins.serclk = GPIO_NUM_14;
    pins.oe = GPIO_NUM_15;
    pins.clr = GPIO_NUM_19;
    pins.rclk = GPIO_NUM_18;
    pins.spinEnable = GPIO_NUM_4;

    Driver* drv = new Driver(&pins, 1);

    drv->set_position(0, 0x66);

    while(true) {
        drv->en();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        drv->dis();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }

}

void main1() {
    driver_pins_t pins;
    pins.ser = GPIO_NUM_13;
    pins.serclk = GPIO_NUM_14;
    pins.oe = GPIO_NUM_15;
    pins.clr = GPIO_NUM_19;
    pins.rclk = GPIO_NUM_18;
    pins.spinEnable = GPIO_NUM_4;

    Driver* drv = new Driver(&pins, 1);
    
    while(true) {
        vTaskDelay(100 / portTICK_PERIOD_MS);

        std::string str;
        
        uint8_t ch = 0;
        while (ch != '\n' && ch != '\r') {
            ETS_STATUS state = uart_rx_one_char(&ch);
            if (ch != '\n' && ch != '\r' && state == ETS_OK) {
                str += ch;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        } 

        if (!str.empty()) {
            if (str == "s") {
                ESP_LOGI("main", "stop");
                drv->dis();
            } else if (str == "g") {
                drv->en(); 
                ESP_LOGI("main", "go");
            } else {

                try {
                    int value = std::stoi(str, nullptr, 10);
                    int gray = gray_table[value];

                    ESP_LOGI("main", "set_position()");
                    ESP_LOG_BUFFER_HEX("main", &gray, 1);
                    drv->set_position(0, gray);

                } catch (void* any) {
                    ESP_LOGE("main", "invalid command");
                }
            }
        }
    }
      //  vTaskDelay(1000 / portTICK_PERIOD_MS);
                
        // for (int i = 0; i < 80; i++) {
        //     drv->en();
        //     ets_delay_us(10000);
        //     drv->dis();
        //     vTaskDelay(1000 / portTICK_PERIOD_MS);
        // }

        
        // drv->set_position(0, 5);
        // ESP_LOGI("MAIN", "Pushing out '5'");
        // vTaskDelay(50000 / portTICK_PERIOD_MS);
}