#include <WidgetInfo.hpp>
#include <AppWindow.hpp>
#include <Common.hpp>


#define WIDGET_INFO_SPACING_LEFT     (20)
#define WIDGET_INFO_SPACING_MIDDLE   (12)
#define WIDGET_INFO_SPACING_RIGHT    (20)
#define WIDGET_INFO_SPACING_SECTION  (20)


WidgetInfo::WidgetInfo(nanogui::Widget *parent): nanogui::Window(parent, "Info"){
    using namespace nanogui;
    this->setPosition(Vector2i(10, 48));
    this->setVisible(false);
    BoxLayout* boxLayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, 12, 12);
    this->setLayout(boxLayout);
    Widget *w, *group;
    group = new Widget(this);
    group->setLayout(new GridLayout(Orientation::Horizontal, 5, Alignment::Fill, 0, 2));


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Section 1: application info
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // AppName
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_LEFT);
    new Label(group,"AppName:","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_MIDDLE);
    new Label(group,strAppName,"sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_RIGHT);

    // Version
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_LEFT);
    new Label(group,"Version:","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_MIDDLE);
    new Label(group,strVersion,"sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_RIGHT);

    // Built
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_LEFT);
    new Label(group,"Built:","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_MIDDLE);
    new Label(group,strBuilt,"sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_RIGHT);


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Section 2: space between section 1 and 3
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_LEFT);
    w->setFixedHeight(WIDGET_INFO_SPACING_SECTION);
    new Widget(group);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_MIDDLE);
    w->setFixedHeight(WIDGET_INFO_SPACING_SECTION);
    new Widget(group);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_RIGHT);
    w->setFixedHeight(WIDGET_INFO_SPACING_SECTION);


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Section 3: GL info
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    std::string versionGL, versionGLSL, vendor, renderer;
    GetGLInfo(&versionGL, &versionGLSL, &vendor, &renderer);

    // GLFW-Version
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_LEFT);
    new Label(group,"GLFW-Version:","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_MIDDLE);
    new Label(group,glfwGetVersionString(),"sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_RIGHT);

    // GL-Version
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_LEFT);
    new Label(group,"GL-Version:","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_MIDDLE);
    new Label(group,versionGL,"sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_RIGHT);

    // GLSL-Version
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_LEFT);
    new Label(group,"GLSL-Version:","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_MIDDLE);
    new Label(group,versionGLSL,"sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_RIGHT);

    // GL-Vendor
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_LEFT);
    new Label(group,"GL-Vendor:","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_MIDDLE);
    new Label(group,vendor,"sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_RIGHT);

    // GL-Renderer
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_LEFT);
    new Label(group,"GL-Renderer:","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_MIDDLE);
    new Label(group,renderer,"sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_INFO_SPACING_RIGHT);


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // End: Close button
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Button* buttonClose = new Button(this, "Close", ENTYPO_ICON_CIRCLE_WITH_CROSS);
    buttonClose->setBackgroundColor(Color(GUI_COLOR_CLOSE_BUTTON));
    buttonClose->setCallback([this](){
        this->setVisible(false);
    });
}

WidgetInfo::~WidgetInfo(){}

