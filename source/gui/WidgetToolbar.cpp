#include <WidgetToolbar.hpp>
#include <AppWindow.hpp>


#define WIDGET_TOOLBAR_ELEMENT_HEIGHT  (30)
#define WIDGET_TOOLBAR_ELEMENT_WIDTH   (120)
#define WIDGET_TOOLBAR_ELEMENT_MARGIN  (2)


WidgetToolbar::WidgetToolbar(nanogui::Widget *parent): nanogui::NonmovableWindow(parent, ""){
    using namespace nanogui;
    this->setPosition(Vector2i(0, 0));

    this->buttonView = new Button(this, "View", ENTYPO_ICON_TV);
    this->buttonView->setFixedSize(Vector2i(WIDGET_TOOLBAR_ELEMENT_WIDTH,WIDGET_TOOLBAR_ELEMENT_HEIGHT));
    this->buttonView->setFontSize(GUI_FONT_SIZE);
    this->buttonView->setCallback([](){
        appWindow.canvas.gui.SetWidgetViewState(true);
    });

    this->buttonVehicles = new Button(this, "Vehicles", ENTYPO_ICON_PAPER_PLANE);
    this->buttonVehicles->setFixedSize(Vector2i(WIDGET_TOOLBAR_ELEMENT_WIDTH,WIDGET_TOOLBAR_ELEMENT_HEIGHT));
    this->buttonVehicles->setFontSize(GUI_FONT_SIZE);
    this->buttonVehicles->setCallback([](){
        appWindow.canvas.gui.SetWidgetVehicleListState(true);
    });

    this->buttonLog = new Button(this, "Log", ENTYPO_ICON_CLIPBOARD);
    this->buttonLog->setFixedSize(Vector2i(WIDGET_TOOLBAR_ELEMENT_WIDTH,WIDGET_TOOLBAR_ELEMENT_HEIGHT));
    this->buttonLog->setFontSize(GUI_FONT_SIZE);
    this->buttonLog->setCallback([this](){
        appWindow.canvas.gui.SetWidgetLogState(true);
        this->logNotification = false;
    });

    this->buttonInfo = new Button(this, "", ENTYPO_ICON_INFO_WITH_CIRCLE);
    this->buttonInfo->setFixedSize(Vector2i(WIDGET_TOOLBAR_ELEMENT_HEIGHT,WIDGET_TOOLBAR_ELEMENT_HEIGHT));
    this->buttonInfo->setFontSize(GUI_FONT_SIZE);
    this->buttonInfo->setCallback([this](){
        appWindow.canvas.gui.SetWidgetInfoState(true);
    });

    this->labelTime = new ClickableLabel(this, "", "sans-bold", 19);
    this->labelTime->setCallback([](){
        appWindow.canvas.gui.SetWidgetTimeState(true);
    });

    this->labelFPS = new Label(this, "", "sans-bold", 19);
    this->logNotification = false;
}

WidgetToolbar::~WidgetToolbar(){}

Eigen::Vector2i WidgetToolbar::preferredSize(NVGcontext *ctx)const{
    glm::ivec2 screenSize = appWindow.GetSize();
    (void)ctx;
    return Eigen::Vector2i(screenSize.x, WIDGET_TOOLBAR_ELEMENT_HEIGHT + 4);
}

void WidgetToolbar::performLayout(NVGcontext *ctx){
    // Layout for base class
    Widget::performLayout(ctx);

    // Buttons
    int xPos = WIDGET_TOOLBAR_ELEMENT_MARGIN;
    this->buttonView->setPosition(Eigen::Vector2i(xPos, WIDGET_TOOLBAR_ELEMENT_MARGIN));
    xPos += WIDGET_TOOLBAR_ELEMENT_WIDTH + WIDGET_TOOLBAR_ELEMENT_MARGIN;
    this->buttonVehicles->setPosition(Eigen::Vector2i(xPos, WIDGET_TOOLBAR_ELEMENT_MARGIN));
    xPos += WIDGET_TOOLBAR_ELEMENT_WIDTH + WIDGET_TOOLBAR_ELEMENT_MARGIN;
    this->buttonLog->setPosition(Eigen::Vector2i(xPos, WIDGET_TOOLBAR_ELEMENT_MARGIN));

    // Time label
    glm::ivec2 screenSize = appWindow.GetSize();
    int w = (int)this->labelTime->width();
    int h = (int)this->labelTime->height();
    this->labelTime->setPosition(Eigen::Vector2i((screenSize.x - w) >> 1, 3 + ((WIDGET_TOOLBAR_ELEMENT_HEIGHT - h) >> 1)));

    // Info button
    w = (int)this->buttonInfo->width() + WIDGET_TOOLBAR_ELEMENT_MARGIN;
    this->buttonInfo->setPosition(Eigen::Vector2i(screenSize.x - w, WIDGET_TOOLBAR_ELEMENT_MARGIN));

    // FPS label
    w += (int)this->labelFPS->width();
    h = (int)this->labelFPS->height();
    this->labelFPS->setPosition(Eigen::Vector2i(screenSize.x - w - 10, 3 + ((WIDGET_TOOLBAR_ELEMENT_HEIGHT - h) >> 1)));
}

void WidgetToolbar::update(NVGcontext *ctx){
    // Update time label
    auto systemClock = std::chrono::system_clock::now();
    std::time_t systemTime = std::chrono::system_clock::to_time_t(systemClock);
    std::tm* localTime = std::localtime(&systemTime);
    char strTime[16];
    sprintf(strTime, "%02d:%02d:%02d",localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
    this->labelTime->setCaption(std::string(strTime));

    // Update FPS label
    uint32_t fps = appWindow.GetFPS();
    this->labelFPS->setCaption(std::to_string(fps) + std::string(" FPS"));

    // Update log button
    if(this->logNotification){
        this->buttonLog->setTextColor(nanogui::Color(255,0,0,255));
    }
    else{
        this->buttonLog->setTextColor(nanogui::Color(255, 160));
    }
    (void)ctx;
}

void WidgetToolbar::SetLogNotification(void){
    this->logNotification = true;
}

