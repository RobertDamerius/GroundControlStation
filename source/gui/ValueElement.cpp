#include <ValueElement.hpp>
#include <GUI.hpp>


#define LABEL_NAME_WIDTH   (70)
#define LABEL_VALUE_WIDTH  (80)
#define LABEL_UNIT_WITH    (50)


ValueElement::ValueElement(nanogui::Widget *parent, std::string name, std::string unit, int precision): nanogui::Widget(parent), precision(precision){
    using namespace nanogui;
    GridLayout *layout = new GridLayout(Orientation::Horizontal, 3, Alignment::Middle, 0, 0);
    layout->setColAlignment({Alignment::Maximum, Alignment::Fill, Alignment::Fill});
    layout->setSpacing(0, 10);
    this->setLayout(layout);

    this->value = NAN;

    labelName = new Label(this, name.c_str(), "sans", GUI_FONT_SIZE);
    labelName->setFixedWidth(LABEL_NAME_WIDTH);
    labelValue = new Label(this, "", "sans", GUI_FONT_SIZE);
    labelValue->setFixedWidth(LABEL_VALUE_WIDTH);
    Label* label = new Label(this, unit.c_str(), "sans", GUI_FONT_SIZE);
    label->setFixedWidth(LABEL_UNIT_WITH);
}

void ValueElement::update(NVGcontext *ctx){
    std::stringstream stream;
    stream << std::fixed << std::setprecision(4) << this->value;
    labelValue->setCaption(stream.str());
    nanogui::Widget::update(ctx);
}

