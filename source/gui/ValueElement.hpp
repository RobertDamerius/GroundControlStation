#pragma once


#include <nanogui.h>


class ValueElement: public nanogui::Widget {
    public:
        int precision;
        double value;

        /**
         *  @brief Create a value element for the vehicle view widget.
         *  @param [in] parent The parent widget.
         *  @param [in] name Name for the value.
         *  @param [in] unit The unit for the value.
         *  @param [in] color The color for the label name, {0,0,0,0} if default color should be used.
         */
        ValueElement(nanogui::Widget *parent, std::string name, std::string unit, int precision = 4, std::array<uint8_t,4> color = {0,0,0,0});

        /**
         *  @brief Update event.
         *  @param [in] ctx NanoVG context.
         */
        void update(NVGcontext *ctx)override;

    protected:
        nanogui::Label* labelName;
        nanogui::Label* labelValue;
};

