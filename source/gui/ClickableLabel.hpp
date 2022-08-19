#pragma once


#include <nanogui.h>


class ClickableLabel: protected nanogui::Button {
    public:
        //using nanogui::Button::operator new;
        using nanogui::Button::setCaption;
        using nanogui::Button::setFontSize;
        using nanogui::Button::setCallback;
        using nanogui::Button::position;
        using nanogui::Button::setPosition;
        using nanogui::Button::absolutePosition;
        using nanogui::Button::size;
        using nanogui::Button::setSize;
        using nanogui::Button::width;
        using nanogui::Button::setWidth;
        using nanogui::Button::height;
        using nanogui::Button::setHeight;

        /**
         *  @brief Create a clickable label.
         *  @param [in] parent Parent widget.
         *  @param [in] caption Caption string.
         *  @param [in] font Font type string.
         *  @param [in] Font size in pixels.
         */
        ClickableLabel(nanogui::Widget* parent, const std::string &caption = "Untitled", const std::string &font = "sans", int fontSize = -1);

        /**
         *  @brief Delete the clickable label.
         */
        ~ClickableLabel();

        /**
         *  @brief Set the font type string.
         *  @param [in] font Font type string.
         */
        void setFont(const std::string &font);

    protected:
        std::string mFont;

        /**
         *  @brief Rendering function.
         *  @param [in] ctx NanoVG context.
         */
        void draw(NVGcontext *ctx)override;
};

