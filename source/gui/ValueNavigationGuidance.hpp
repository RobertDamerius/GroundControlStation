#pragma once


#include <nanogui.h>


class ValueNavigationGuidance: public nanogui::Widget {
    public:
        double guidance;
        double navigation;

        /**
         *  @brief Create a guidance/navigation value for the vehicle view widget.
         *  @param [in] parent The parent widget.
         *  @param [in] name Name for the value.
         *  @param [in] unit The unit for the value.
         */
        ValueNavigationGuidance(nanogui::Widget *parent, std::string name, std::string unit);

        /**
         *  @brief Update event.
         *  @param [in] ctx NanoVG context.
         */
        void update(NVGcontext *ctx)override;

    protected:
        nanogui::Label* labelName;
        nanogui::Label* labelNavigation;
        nanogui::Label* labelGuidance;
};

