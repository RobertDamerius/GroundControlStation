#include <WidgetLog.hpp>
#include <AppWindow.hpp>
#include <Common.hpp>


WidgetLog::WidgetLog(nanogui::Widget *parent): nanogui::Window(parent, "Log"){
    using namespace nanogui;
    this->setPosition(Vector2i(10, 48));
    this->setVisible(false);
    BoxLayout* boxLayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, 12, 12);
    this->setLayout(boxLayout);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // VScroll
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    VScrollPanel* scrollPanel = new VScrollPanel(this);
    scrollPanel->setFixedWidth(1024);
    scrollPanel->setFixedHeight(300);
    Widget* scrollContent = new Widget(scrollPanel);
    scrollContent->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 0, 0));

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Log table, initialize with header
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    logContent = new Widget(scrollContent);
    logContent->setLayout(new GridLayout(Orientation::Horizontal, 5, Alignment::Fill, 0, 0));
    new Label(logContent,"UTC","sans-bold",GUI_FONT_SIZE);
    new Label(logContent,"   ","sans-bold",GUI_FONT_SIZE);
    new Label(logContent,"Source","sans-bold",GUI_FONT_SIZE);
    new Label(logContent,"   ","sans-bold",GUI_FONT_SIZE);
    new Label(logContent,"Message                                                          "\
                "                                                                 "\
                "                                                                 "\
                "                                                                 "\
                "          ","sans-bold",GUI_FONT_SIZE);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // End: Close button
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Button* buttonClose = new Button(this, "Close", ENTYPO_ICON_CIRCLE_WITH_CROSS);
    buttonClose->setBackgroundColor(Color(GUI_COLOR_CLOSE_BUTTON));
    buttonClose->setCallback([this](){
        this->setVisible(false);
    });
}

WidgetLog::~WidgetLog(){}

void WidgetLog::update(NVGcontext *ctx){
    using namespace nanogui;
    mtx.lock();
    for(auto&& log : logsToAdd){
        Color clr(log.color[0], log.color[1], log.color[2], 255);
        Label *label = new Label(logContent,log.timestamp,"sans",GUI_FONT_SIZE);
        label->setColor(clr);
        new Label(logContent,"","sans-bold",GUI_FONT_SIZE);
        label = new Label(logContent,log.source,"sans",GUI_FONT_SIZE);
        label->setColor(clr);
        new Label(logContent,"","sans-bold",GUI_FONT_SIZE);
        label = new Label(logContent,log.text,"sans",GUI_FONT_SIZE);
        label->setColor(clr);
    }
    logsToAdd.clear();
    mtx.unlock();
    (void)ctx;
}

void WidgetLog::AddLogEntry(const LogEntry& log){
    mtx.lock();
    logsToAdd.push_back(log);
    mtx.unlock();
}

