#include <ValueNavigationGuidance.hpp>
#include <GUI.hpp>


#define LABEL_NAME_WIDTH   (70)
#define LABEL_VALUE_WIDTH  (80)
#define LABEL_UNIT_WITH    (50)


ValueNavigationGuidance::ValueNavigationGuidance(nanogui::Widget *parent, std::string name, std::string unit): nanogui::Widget(parent){
    using namespace nanogui;
    GridLayout *layout = new GridLayout(Orientation::Horizontal, 3, Alignment::Middle, 0, 0);
    layout->setColAlignment({Alignment::Maximum, Alignment::Fill, Alignment::Fill});
    layout->setSpacing(0, 10);
    this->setLayout(layout);

    this->guidance = NAN;
    this->navigation = NAN;
    this->guidanceActive = false;
    this->navigationActive = false;

    // Name
    labelName = new Label(this, name.c_str(), "sans", GUI_FONT_SIZE);
    labelName->setFixedWidth(LABEL_NAME_WIDTH);

    // Navication: numeric value + unit
    labelNavigation = new Label(this, "", "sans", GUI_FONT_SIZE);
    labelNavigation->setFixedWidth(LABEL_VALUE_WIDTH);
    labelNavigation->setColor(Color(200,200,200,255));
    labelUnitNavigation = new Label(this, unit.c_str(), "sans", GUI_FONT_SIZE);
    labelUnitNavigation->setFixedWidth(LABEL_UNIT_WITH);
    labelUnitNavigation->setColor(Color(200,200,200,255));
    Widget* widget = new Widget(this);
    widget->setFixedWidth(LABEL_NAME_WIDTH);

    // Guidance: numeric value + unit
    labelGuidance = new Label(this, "", "sans", GUI_FONT_SIZE);
    labelGuidance->setColor(Color(GUI_TEXT_COLOR_GUIDANCE_R,GUI_TEXT_COLOR_GUIDANCE_G,GUI_TEXT_COLOR_GUIDANCE_B,255));
    labelGuidance->setFixedWidth(LABEL_VALUE_WIDTH);
    labelUnitGuidance = new Label(this, unit.c_str(), "sans", GUI_FONT_SIZE);
    labelUnitGuidance->setColor(Color(GUI_TEXT_COLOR_GUIDANCE_R,GUI_TEXT_COLOR_GUIDANCE_G,GUI_TEXT_COLOR_GUIDANCE_B,255));
    labelUnitGuidance->setFixedWidth(LABEL_UNIT_WITH);
}

void ValueNavigationGuidance::update(NVGcontext *ctx){
    using namespace nanogui;

    // Generate limited strings for navigation and guidance values
    std::stringstream streamNav;
    streamNav << std::fixed << std::setprecision(3) << this->navigation;
    std::string strNavigation = streamNav.str();
    if(strNavigation.length() > 10){
        std::stringstream ss;
        ss << std::setprecision(2) << this->navigation;
        strNavigation = ss.str();
    }
    std::stringstream streamGui;
    streamGui << std::fixed << std::setprecision(3) << this->guidance;
    std::string strGuidance = streamGui.str();
    if(strGuidance.length() > 10){
        std::stringstream ss;
        ss << std::setprecision(2) << this->guidance;
        strGuidance = ss.str();
    }

    // Update GUI labels
    labelNavigation->setCaption(strNavigation);
    labelNavigation->setColor(Color(200,200,200, this->navigationActive ? 255 : 100));
    labelUnitNavigation->setColor(Color(200,200,200, this->navigationActive ? 255 : 100));
    labelGuidance->setCaption(strGuidance);
    labelGuidance->setColor(Color(GUI_TEXT_COLOR_GUIDANCE_R,GUI_TEXT_COLOR_GUIDANCE_G,GUI_TEXT_COLOR_GUIDANCE_B,this->guidanceActive ? 255 : 100));
    labelUnitGuidance->setColor(Color(GUI_TEXT_COLOR_GUIDANCE_R,GUI_TEXT_COLOR_GUIDANCE_G,GUI_TEXT_COLOR_GUIDANCE_B,this->guidanceActive ? 255 : 100));
    nanogui::Widget::update(ctx);
}

