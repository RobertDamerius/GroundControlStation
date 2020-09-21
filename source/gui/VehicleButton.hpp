#pragma once


#include <nanogui/nanogui.h>
#include <VehicleID.hpp>
#include <WidgetVehicle.hpp>


class VehicleButton : public nanogui::Widget {
    public:
        /**
         *  @brief Create a vehicle button.
         *  @param [in] parent Parent widget.
         *  @param [in] screen Screen widget.
         *  @param [in] id Vehicle ID.
         */
        VehicleButton(nanogui::Widget *parent, nanogui::Widget *screen, const VehicleID id);

        /**
         *  @brief The preferred size of this button.
         *  @param [in] ctx NanoVG context.
         *  @return Preferred size in pixels.
         */
        Eigen::Vector2i preferredSize(NVGcontext *ctx)const override;

        /**
         *  @brief The callback that is called when any type of mouse button event is issued to this button.
         *  @param [in] p Mouse cursor position.
         *  @param [in] button GLFW button.
         *  @param [in] down True if pressed, false otherwise.
         *  @param [in] modifiers Modifier keys.
         *  @return True if button was pressed, false otherwise.
         */
        bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers)override;

        /**
         *  @brief Draw the button.
         *  @param [in] ctx NanoVG context.
         */
        void draw(NVGcontext *ctx)override;

    protected:
        VehicleID id;                    ///< The vehicle ID.
        std::string mCaptionName;        ///< Caption for name string of this button.
        std::string mCaptionDescription; ///< Caption for description string of this button.
        bool mPushed;                    ///< True if button is currently pushed, false otherwise.
        std::function<void()> mCallback; ///< The push callback function.
        WidgetVehicle* widgetVehicle;    ///< The vehicle widget.
};

