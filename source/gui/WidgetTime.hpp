#pragma once


#include <nanogui.h>


class WidgetTime: public nanogui::Window {
    public:
        /**
         *  @brief Create a time widget.
         *  @param [in] parent The parent widget.
         */
        explicit WidgetTime(nanogui::Widget *parent);

        /**
         *  @brief Delete the time widget.
         */
        ~WidgetTime();

        /**
         *  @brief Update event.
         *  @param [in] ctx NanoVG context.
         */
        void update(NVGcontext *ctx)override;

    private:
        nanogui::Label* labelLocal;
        nanogui::Label* labelUTC;
        nanogui::Label* labelUTCSOD;
        nanogui::Label* labelUNIX;
};

