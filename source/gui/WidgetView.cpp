#include <WidgetView.hpp>
#include <AppWindow.hpp>
#include <Common.hpp>


#define WIDGET_VIEW_SECTION_MARGIN            (8)
#define WIDGET_VIEW_SECTION_SPACING           (8)
#define WIDGET_VIEW_SECTION_GROUP_SPACING     (0)

#define WIDGET_VIEW_GROUP_MARGIN              (10)
#define WIDGET_VIEW_GROUP_SPACING             (4)

#define WIDGET_VIEW_GROUP_WIDTH_LEFT          (100)
#define WIDGET_VIEW_GROUP_WIDTH_RIGHT         (120)
#define WIDGET_VIEW_GROUP_HEIGHT_ELEMENT      (20)


WidgetView::WidgetView(nanogui::Widget *parent): nanogui::Window(parent, "View"){
    // Widget settings
    using namespace nanogui;
    this->setPosition(Vector2i(10, 48));
    this->setVisible(false);
    BoxLayout* boxLayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, WIDGET_VIEW_SECTION_MARGIN, WIDGET_VIEW_SECTION_SPACING);
    this->setLayout(boxLayout);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Group: Origin
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    {
        Widget* group = new Widget(this);
        boxLayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, 0, WIDGET_VIEW_SECTION_GROUP_SPACING);
        group->setLayout(boxLayout);
        Label* label = new Label(group, "Geographic Origin", "sans-bold");
        label->setFontSize(GUI_FONT_SIZE);
        Widget* widget = new Widget(group);
        GridLayout* layout = new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, WIDGET_VIEW_GROUP_MARGIN, WIDGET_VIEW_GROUP_SPACING);
        layout->setColAlignment({Alignment::Maximum, Alignment::Fill});
        widget->setLayout(layout);

        // Latitude
        label = new Label(widget, "Latitude", "sans");
        label->setFontSize(GUI_FONT_SIZE);
        label->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_LEFT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        originLat = new Label(widget, "undefined", "sans");
        label->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        label->setFontSize(GUI_FONT_SIZE);

        // Longitude
        label = new Label(widget, "Longitude", "sans");
        label->setFontSize(GUI_FONT_SIZE);
        label->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_LEFT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        originLon = new Label(widget, "undefined", "sans");
        label->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        label->setFontSize(GUI_FONT_SIZE);

        // Altitude
        label = new Label(widget, "Altitude", "sans");
        label->setFontSize(GUI_FONT_SIZE);
        label->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_LEFT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        originAlt = new Label(widget, "undefined", "sans");
        label->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        label->setFontSize(GUI_FONT_SIZE);
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Group: Camera
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    {
        Widget* group = new Widget(this);
        boxLayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, 0, WIDGET_VIEW_SECTION_GROUP_SPACING);
        group->setLayout(boxLayout);
        Label* label = new Label(group, "Camera", "sans-bold");
        label->setFontSize(GUI_FONT_SIZE);
        Widget* widget = new Widget(group);
        GridLayout* layout = new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, WIDGET_VIEW_GROUP_MARGIN, WIDGET_VIEW_GROUP_SPACING);
        layout->setColAlignment({Alignment::Maximum, Alignment::Fill});
        widget->setLayout(layout);

        // Mode
        label = new Label(widget, "Mode", "sans");
        label->setFontSize(GUI_FONT_SIZE);
        label->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_LEFT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        ComboBox* comboBox = new ComboBox(widget, {"3D","2D"});
        comboBox->setFontSize(GUI_FONT_SIZE);
        comboBox->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        comboBox->setSelectedIndex((appWindow.canvas.scene.viewCamera.mode == CAMERA_MODE_PERSPECTIVE) ? 0 : 1);
        comboBox->setCallback([](int i){
            switch(i){
                case 0: appWindow.canvas.scene.SetCameraMode(CAMERA_MODE_PERSPECTIVE); break;
                case 1: appWindow.canvas.scene.SetCameraMode(CAMERA_MODE_ORTHOGRAPHIC); break;
            }
        });

        // Lock
        label = new Label(widget, "Vehicle Lock", "sans");
        label->setFontSize(GUI_FONT_SIZE);
        label->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_LEFT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        Button* buttonUnlock = new Button(widget, "Unlock", ENTYPO_ICON_LOCK_OPEN);
        buttonUnlock->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        buttonUnlock->setFontSize(GUI_FONT_SIZE);
        buttonUnlock->setCallback([](){
            appWindow.canvas.scene.humanInterface.cameraLock.Reset();
        });
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Group: Display
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    {
        Widget* group = new Widget(this);
        boxLayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, 0, WIDGET_VIEW_SECTION_GROUP_SPACING);
        group->setLayout(boxLayout);
        Label* label = new Label(group, "Display", "sans-bold");
        label->setFontSize(GUI_FONT_SIZE);
        Widget* widget = new Widget(group);
        GridLayout* layout = new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, WIDGET_VIEW_GROUP_MARGIN, WIDGET_VIEW_GROUP_SPACING);
        layout->setColAlignment({Alignment::Maximum, Alignment::Fill});
        widget->setLayout(layout);

        // Gamma
        label = new Label(widget, "Gamma", "sans");
        label->setFontSize(GUI_FONT_SIZE);
        label->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_LEFT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        Slider* sliderGamma = new Slider(widget);
        sliderGamma->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        sliderGamma->setRange(std::pair<float, float>(1.0f, 3.0f));
        sliderGamma->setValue((float)appWindow.canvas.renderer.GetGamma());
        sliderGamma->setCallback([](float f){
            appWindow.canvas.renderer.SetGamma((GLfloat)f);
        });
        sliderGamma->setFinalCallback([](float f){
            appWindow.canvas.renderer.SetGamma((GLfloat)f);
        });
        new Widget(widget);
        Button* buttonGamma = new Button(widget, "Set Default");
        buttonGamma->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        buttonGamma->setFontSize(GUI_FONT_SIZE);
        buttonGamma->setCallback([sliderGamma](){
            appWindow.canvas.renderer.SetGamma((GLfloat)GAMMA_DEFAULT);
            sliderGamma->setValue((float)appWindow.canvas.renderer.GetGamma());
        });

        // Ground color
        label = new Label(widget, "Ground Color", "sans");
        label->setFontSize(GUI_FONT_SIZE);
        label->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_LEFT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        ColorPicker* cp = new ColorPicker(widget);
        cp->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        cp->setColor(Color((float)sqrt(appWindow.canvas.scene.groundPlane.color.x), (float)sqrt(appWindow.canvas.scene.groundPlane.color.y), (float)sqrt(appWindow.canvas.scene.groundPlane.color.z), 1.0f));
        cp->setCallback([cp](const Color& c){
            glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
            appWindow.canvas.scene.groundPlane.color = appWindow.canvas.renderer.SetGroundPlaneColor(clr);
            cp->setColor(Color((float)sqrt(appWindow.canvas.scene.groundPlane.color.x), (float)sqrt(appWindow.canvas.scene.groundPlane.color.y), (float)sqrt(appWindow.canvas.scene.groundPlane.color.z), 1.0f));
        });
        cp->setFinalCallback([cp](const Color &c) {
            glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
            appWindow.canvas.scene.groundPlane.color = appWindow.canvas.renderer.SetGroundPlaneColor(clr);
            cp->setColor(Color((float)sqrt(appWindow.canvas.scene.groundPlane.color.x), (float)sqrt(appWindow.canvas.scene.groundPlane.color.y), (float)sqrt(appWindow.canvas.scene.groundPlane.color.z), 1.0f));
        });
        new Widget(widget);
        Button* buttonGPC = new Button(widget, "Set Default");
        buttonGPC->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
        buttonGPC->setFontSize(GUI_FONT_SIZE);
        buttonGPC->setCallback([cp](){
            appWindow.canvas.scene.groundPlane.Reset();
            appWindow.canvas.scene.groundPlane.color = appWindow.canvas.renderer.SetGroundPlaneColor(appWindow.canvas.scene.groundPlane.color);
            cp->setColor(Color((float)sqrt(appWindow.canvas.scene.groundPlane.color.x), (float)sqrt(appWindow.canvas.scene.groundPlane.color.y), (float)sqrt(appWindow.canvas.scene.groundPlane.color.z), 1.0f));
        });
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // End: Close button
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Button* buttonClose = new Button(this, "Close", ENTYPO_ICON_CIRCLE_WITH_CROSS);
    buttonClose->setBackgroundColor(Color(GUI_COLOR_CLOSE_BUTTON));
    buttonClose->setCallback([this](){
        this->setVisible(false);
    });
}

WidgetView::~WidgetView(){}

void WidgetView::update(NVGcontext *ctx){
    using namespace nanogui;
    double lat, lon, alt;
    appWindow.canvas.scene.origin.Get(lat, lon, alt);
    lat *= 57.29577951308232088;
    lon *= 57.29577951308232088;
    std::string strLat = std::to_string(std::abs(lat)) + ((lat < 0.0) ? std::string(" S") : std::string(" N"));
    std::string strLon = std::to_string(std::abs(lon)) + ((lon < 0.0) ? std::string(" W") : std::string(" E"));
    this->originLat->setCaption(strLat);
    this->originLat->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
    this->originLon->setCaption(strLon);
    this->originLon->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
    this->originAlt->setCaption(std::to_string(alt));
    this->originAlt->setFixedSize(Vector2i(WIDGET_VIEW_GROUP_WIDTH_RIGHT,WIDGET_VIEW_GROUP_HEIGHT_ELEMENT));
    (void)ctx;
}

