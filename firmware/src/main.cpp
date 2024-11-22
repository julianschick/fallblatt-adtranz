#include "driver.h"

#include <esp_log.h>
#include <rom/ets_sys.h>
#include <rom/uart.h>

#include <string>
#include <stdexcept>

void main1();
void main2();


extern "C" void app_main() {
    main2();
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
    
    // 0x01 c
    // 0x03 K
    // 0x04 )
    // 0x05 n
    // 0x07 9
    // 0x09 Ä
    // 0x0B P
    // 0x0C void
    // 0x0D s
    // 0x0F 4
    // 0x10 =
    // 0x11 f
    // 0x13 H
    // 0x14 *
    // 0x15 k
    // 0x17 C
    // 0x19 X
    // 0x1B S
    // 0x1C ä
    // 0x1D v
    // 0x1F 1
    // 0x21 b
    // 0x23 L
    // 0x24 (
    // 0x25 o
    // 0x27 8
    // 0x29 Ö
    // 0x2B O
    // 0x2C -
    // 0x2D r
    // 0x2F 5
    // 0x30 "
    // 0x31 g
    // 0x33 G
    // 0x34 :
    // 0x35 j
    // 0x37 D
    // 0x39 W
    // 0x3B T
    // 0x3C z
    // 0x3D w
    // 0x3F 0
    // 0x41 d
    // 0x43 J
    // 0x44 ?
    // 0x45 m
    // 0x47 A
    // 0x49 Z
    // 0x4B Q
    // 0x4C ü
    // 0x4D t
    // 0x4F 3
    // 0x50 +
    // 0x51 e
    // 0x53 I
    // 0x54 !
    // 0x55 l
    // 0x57 B
    // 0x59 Y
    // 0x5B R
    // 0x5C ö
    // 0x5D u
    // 0x5F 2
    // 0x61 a
    // 0x63 M
    // 0x64 ,
    // 0x65 p
    // 0x67 7
    // 0x69 Ü
    // 0x6B N
    // 0x6C .
    // 0x6D q
    // 0x6F 6
    // 0x70 |
    // 0x71 h
    // 0x73 F
    // 0x74 /
    // 0x75 i
    // 0x77 E
    // 0x79 V
    // 0x7B U
    // 0x7C y
    // 0x7D x
    // 0x7F void



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
                    uint32_t value = std::stoul(str, nullptr, 16);

                    ESP_LOGI("main", "set_position()");
                    ESP_LOG_BUFFER_HEX("main", &value, 1);
                    drv->set_position(0, value);

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