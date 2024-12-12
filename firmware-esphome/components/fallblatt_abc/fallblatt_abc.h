#ifndef FALLBLATT_ABC_H_
#define FALLBLATT_ABC_H_

#include <esphome.h>
#include <esphome/core/component.h>
#include <esphome/components/text/text.h>

namespace esphome::fallblatt {

    class FallblattABC : public Component, public esphome::text::Text {
    public:
        void setup() override;
        void loop() override;
        void control(const std::string &value) override;

    protected:
        Driver* drv;

    };
}

#endif