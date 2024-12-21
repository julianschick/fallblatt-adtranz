#include "fallblatt_abc.h"

#include <nvs_flash.h>
#include <nvs.h>

#include "gray.h"

#define LOG_TAG "ABC"

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

        state = "";
        load_from_nvs();
        control(state);
    }

    void FallblattABC::loop() {
        drv->loop();
    }

    void FallblattABC::control(const std::string &value) {
        publish_state(value);
        state = value;
        
        std::string latinValue = GrayConverter::utf8ToLatin(value);

        int n = number_of_modules;

        for (int i = 0; i < n; i++) {
            if (i < latinValue.size()) {
                int pos = GrayConverter::charToPosition(latinValue[i]);
                drv->set_position(n - i - 1, pos);
            } else {
                drv->set_position(n - i - 1, GrayConverter::charToPosition(' '));
            }
        }

        drv->update();
        
        ESP_LOGI(LOG_TAG, "Splitflap message has been set: '%s'", value.c_str());
        store_in_nvs();
    }

    void FallblattABC::load_from_nvs() {
        nvs_handle_t handle;
        
        esp_err_t ret = nvs_open("abc", NVS_READONLY, &handle);
        if (ret != ESP_OK) { return; }

        char* buf = new char[255];
        size_t maxlen = 255;
        ret = nvs_get_str(handle, "msg", buf, &maxlen);
        nvs_close(handle);

        if (ret == ESP_OK) {
            ESP_LOGI(LOG_TAG, "Restored last displayed splitflap message: '%s'", buf);
            state = std::string(buf).substr(0, number_of_modules);
        }

        delete[] buf;
    }

    void FallblattABC::store_in_nvs() {
        nvs_handle_t handle;
        ESP_ERROR_CHECK(nvs_open("abc", NVS_READWRITE, &handle));
        ESP_ERROR_CHECK(nvs_set_str(handle, "msg", state.substr(0, number_of_modules).c_str()));
        nvs_close(handle);
    }
}