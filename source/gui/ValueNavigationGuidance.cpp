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

    labelName = new Label(this, name.c_str(), "sans", GUI_FONT_SIZE);
    labelName->setFixedWidth(LABEL_NAME_WIDTH);
    labelNavigation = new Label(this, "", "sans", GUI_FONT_SIZE);
    labelNavigation->setFixedWidth(LABEL_VALUE_WIDTH);
    Label* label = new Label(this, unit.c_str(), "sans", GUI_FONT_SIZE);
    label->setFixedWidth(LABEL_UNIT_WITH);
    Widget* widget = new Widget(this);
    widget->setFixedWidth(LABEL_NAME_WIDTH);
    labelGuidance = new Label(this, "", "sans", GUI_FONT_SIZE);
    labelGuidance->setColor(Color(30,190,255,255));
    labelGuidance->setFixedWidth(LABEL_VALUE_WIDTH);
    label = new Label(this, unit.c_str(), "sans", GUI_FONT_SIZE);
    label->setColor(Color(30,190,255,255));
    label->setFixedWidth(LABEL_UNIT_WITH);
}

void ValueNavigationGuidance::update(NVGcontext *ctx){
    labelNavigation->setCaption(std::to_string(this->navigation));
    labelGuidance->setCaption(std::to_string(this->guidance));
    nanogui::Widget::update(ctx);
}

