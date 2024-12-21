#ifndef FALLBLATT_ABC_H_
#define FALLBLATT_ABC_H_

#include <esphome.h>
#include <esphome/core/component.h>
#include <esphome/components/text/text.h>

namespace esphome::fallblatt {

    class FallblattABC : public Component, public text::Text {
    public:
        FallblattABC(int number_of_modules) : Component(), text::Text(), number_of_modules(number_of_modules) { }

        void setup() override;
        void loop() override;
        void control(const std::string &value) override;

    protected:
        int number_of_modules;

        Driver* drv;

    };
}

#endif