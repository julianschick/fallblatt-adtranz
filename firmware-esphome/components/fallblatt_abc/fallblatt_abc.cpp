#include "fallblatt_abc.h"

#include "gray.h"

namespace esphome::fallblatt {

    void FallblattABC::setup() {
        driver_pins_t pins;
        pins.ser = GPIO_NUM_13;
        pins.serclk = GPIO_NUM_14;
        pins.oe = GPIO_NUM_15;
        pins.clr = GPIO_NUM_19;
        pins.rclk = GPIO_NUM_18;
        pins.spin_enable = GPIO_NUM_4;
        pins.power = GPIO_NUM_2;

        drv = new Driver(&pins, 1);

        drv->set_position(0, 0);
        drv->en();

        publish_state("");
    }

    void FallblattABC::loop() {
        
    }

    void FallblattABC::control(const std::string &value) {
        publish_state(value);
        state = value;

        std::string latinValue = GrayConverter::utf8ToLatin(value);

        ESP_LOGI("ABC", "strlen = %d", latinValue.size());
        for (int i = 0; i < latinValue.size(); i++) {
            ESP_LOGI("ABC", "str[%d] = %d", i, latinValue[i]);
        }

        if (latinValue.size() > 0) {
            int pos = GrayConverter::charToPosition(latinValue[0]);
            ESP_LOGI("ABC", "pos1 = %d", pos);
            drv->set_position(0, pos);
        }
        
        ESP_LOGI("ABC", "Text has been set: %s", value.c_str());
    }

}