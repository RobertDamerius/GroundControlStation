#pragma once


#include <nanogui.h>
#include <VehicleID.hpp>
#include <VehicleButton.hpp>


class WidgetVehicleList: public nanogui::Window {
    public:
        /**
         *  @brief Create a vehicle list widget.
         *  @param [in] parent The parent widget.
         */
        explicit WidgetVehicleList(nanogui::Widget *parent);

        /**
         *  @brief Delete the vehicle list widget.
         */
        ~WidgetVehicleList();

        /**
         *  @brief Update the @ref elements.
         *  @details New elements may be added.
         */
        void UpdateElements(void);

    protected:
        nanogui::Widget* scrollContent;
        std::unordered_map<VehicleID, VehicleButton*> elements;
};

