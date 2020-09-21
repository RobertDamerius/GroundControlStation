#pragma once


#include <NonmovableWindow.hpp>
#include <nanogui.h>
#include <ClickableLabel.hpp>


class WidgetToolbar: public nanogui::NonmovableWindow {
    public:
        /**
         *  @brief Create a toolbar widget.
         *  @param [in] parent The parent widget.
         */
        explicit WidgetToolbar(nanogui::Widget *parent);

        /**
         *  @brief Delete the toolbar widget.
         */
        ~WidgetToolbar();

        /**
         *  @brief The preferred size of this button.
         *  @param [in] ctx NanoVG context.
         *  @return Preferred size in pixels.
         */
        Eigen::Vector2i preferredSize(NVGcontext *ctx)const override;

        /**
         *  @brief Perform layout.
         *  @param [in] ctx NanoVG context.
         */
        void performLayout(NVGcontext *ctx)override;

        /**
         *  @brief Update event.
         *  @param [in] ctx NanoVG context.
         */
        void update(NVGcontext *ctx)override;

        /**
         *  @brief Set the log notification for the toolbar widget.
         *  @details This member function can be called from any thread.
         */
        void SetLogNotification(void);

    private:
        nanogui::Button* buttonView;
        nanogui::Button* buttonVehicles;
        nanogui::Button* buttonLog;
        nanogui::Button* buttonInfo;
        ClickableLabel* labelTime;
        nanogui::Label* labelFPS;
        std::atomic<bool> logNotification;
};

