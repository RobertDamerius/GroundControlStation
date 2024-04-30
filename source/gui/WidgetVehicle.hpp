#pragma once


#include <nanogui.h>
#include <VehicleID.hpp>
#include <ValueNavigationGuidance.hpp>
#include <ValueElement.hpp>


class WidgetVehicle: public nanogui::Window {
    public:
        /**
         *  @brief Create a vehicle widget.
         *  @param [in] parent The parent widget.
         *  @param [in] id The vehicle ID.
         */
        WidgetVehicle(nanogui::Widget *parent, const VehicleID id);

        /**
         *  @brief Delete the vehicle widget.
         */
        ~WidgetVehicle();

        /**
         *  @brief Update event.
         *  @param [in] ctx NanoVG context.
         */
        void update(NVGcontext *ctx)override;

    protected:
        VehicleID id;
        ValueNavigationGuidance* latitude;
        ValueNavigationGuidance* longitude;
        ValueNavigationGuidance* altitude;
        ValueNavigationGuidance* positionX;
        ValueNavigationGuidance* positionY;
        ValueNavigationGuidance* positionZ;
        ValueNavigationGuidance* roll;
        ValueNavigationGuidance* pitch;
        ValueNavigationGuidance* yaw;
        ValueNavigationGuidance* velocityNorth;
        ValueNavigationGuidance* velocityEast;
        ValueNavigationGuidance* velocityDown;
        ValueNavigationGuidance* velocityU;
        ValueNavigationGuidance* velocityV;
        ValueNavigationGuidance* velocityW;
        ValueNavigationGuidance* velocityP;
        ValueNavigationGuidance* velocityQ;
        ValueNavigationGuidance* velocityR;
        ValueElement* timeNavigation;
        ValueElement* timeGuidance;
        ValueElement* timeGuidanceTrajectory;
        ValueElement* timeGuidanceWaypoint;
        ValueElement* timePolygon;
        nanogui::Button* buttonSave;
};

