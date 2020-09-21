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
         */
        ValueElement(nanogui::Widget *parent, std::string name, std::string unit, int precision = 4);

        /**
         *  @brief Update event.
         *  @param [in] ctx NanoVG context.
         */
        void update(NVGcontext *ctx)override;

    protected:
        nanogui::Label* labelName;
        nanogui::Label* labelValue;
};

