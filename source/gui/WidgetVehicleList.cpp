#include <WidgetVehicleList.hpp>
#include <GUI.hpp>
#include <VehicleButton.hpp>
#include <AppWindow.hpp>


WidgetVehicleList::WidgetVehicleList(nanogui::Widget *parent): nanogui::Window(parent, "Vehicle List"){
    // Widget settings
    using namespace nanogui;
    this->setPosition(Vector2i(10, 48));
    this->setVisible(false);
    this->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 8, 8));

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // VScroll
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    VScrollPanel* scrollPanel = new VScrollPanel(this);
    scrollPanel->setFixedHeight(300);
    scrollContent = new Widget(scrollPanel);
    scrollContent->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 5, 5));

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Close button
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Button* buttonClose = new Button(this, "Close", ENTYPO_ICON_CIRCLE_WITH_CROSS);
    buttonClose->setBackgroundColor(Color(GUI_COLOR_CLOSE_BUTTON));
    buttonClose->setCallback([this](){
        this->setVisible(false);
    });
}

WidgetVehicleList::~WidgetVehicleList(){}

void WidgetVehicleList::UpdateElements(void){
    // Get the current list of vehicles from the vehicle manager
    std::vector<std::tuple<VehicleID, RigidBodyState, RigidBodyState>> vehicles;
    appWindow.canvas.scene.vehicleManager.GetVehiclesList(vehicles);

    // Go through all vehicles and check if they are already in the element container
    for(auto&& v : vehicles){
        auto& vehicleID = std::get<0>(v);
        auto got = this->elements.find(vehicleID);
        if(got == this->elements.end()){
            // Add new entry to the element container
            VehicleButton* vb = new VehicleButton(this->scrollContent, this->mParent, vehicleID);
            this->elements.insert(std::pair<VehicleID, VehicleButton*>(vehicleID, vb));
        }
    }
}

