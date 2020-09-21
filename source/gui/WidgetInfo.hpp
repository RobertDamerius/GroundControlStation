#pragma once


#include <nanogui.h>


class WidgetInfo: public nanogui::Window {
    public:
        /**
         *  @brief Create a info widget.
         *  @param [in] parent The parent widget.
         */
        explicit WidgetInfo(nanogui::Widget *parent);

        /**
         *  @brief Delete the info widget.
         */
        ~WidgetInfo();
};

