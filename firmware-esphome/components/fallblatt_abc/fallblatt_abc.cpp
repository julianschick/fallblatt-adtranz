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

        drv = new Driver(&pins, number_of_modules);

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

        int n = drv->get_device_count();

        for (int i = 0; i < n; i++) {
            if (i < latinValue.size()) {
                ESP_LOGI("ABC", "str[%d] = %d", i, latinValue[i]);
                int pos = GrayConverter::charToPosition(latinValue[i]);
                ESP_LOGI("ABC", "pos[%d] = %d", i, pos);
                drv->set_position(n - i - 1, pos);
            } else {
                drv->set_position(n - i - 1, GrayConverter::charToPosition(' '));
            }
        }
        
        ESP_LOGI("ABC", "Text has been set: %s", value.c_str());
    }

}