#pragma once


#include <nanogui.h>


class WidgetView: public nanogui::Window {
    public:
        /**
         *  @brief Create a view widget.
         *  @param [in] parent The parent widget.
         */
        explicit WidgetView(nanogui::Widget *parent);

        /**
         *  @brief Delete the view widget.
         */
        ~WidgetView();

        /**
         *  @brief Update event.
         *  @param [in] ctx NanoVG context.
         */
        void update(NVGcontext *ctx)override;

    private:
        nanogui::Label* originLat;
        nanogui::Label* originLon;
        nanogui::Label* originAlt;
};

