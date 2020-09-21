#include <WidgetTime.hpp>
#include <AppWindow.hpp>
#include <Common.hpp>


#define WIDGET_TIME_SPACING_LEFT     (20)
#define WIDGET_TIME_SPACING_MIDDLE   (12)
#define WIDGET_TIME_SPACING_RIGHT    (20)


WidgetTime::WidgetTime(nanogui::Widget *parent): nanogui::Window(parent, "Time"){
    using namespace nanogui;
    this->setPosition(Vector2i(10, 48));
    this->setVisible(false);
    BoxLayout* boxLayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, 12, 12);
    this->setLayout(boxLayout);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Group: time info
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Widget *w, *group;
    group = new Widget(this);
    group->setLayout(new GridLayout(Orientation::Horizontal, 5, Alignment::Fill, 0, 2));

    // Local time
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_LEFT);
    new Label(group,"Local Time:","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_MIDDLE);
    labelLocal = new Label(group,"yyyy-mm-dd   hh:mm:ss   DST=x","sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_RIGHT);

    // UTC
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_LEFT);
    new Label(group,"UTC:","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_MIDDLE);
    labelUTC = new Label(group,"","sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_RIGHT);

    // UTC (second of day)
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_LEFT);
    new Label(group,"Second of day (UTC):","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_MIDDLE);
    labelUTCSOD = new Label(group,"","sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_RIGHT);

    // UNIX
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_LEFT);
    new Label(group,"UNIX:","sans-bold",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_MIDDLE);
    labelUNIX = new Label(group,"","sans",GUI_FONT_SIZE);
    w = new Widget(group);
    w->setFixedWidth(WIDGET_TIME_SPACING_RIGHT);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // End: Close button
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Button* buttonClose = new Button(this, "Close", ENTYPO_ICON_CIRCLE_WITH_CROSS);
    buttonClose->setBackgroundColor(Color(GUI_COLOR_CLOSE_BUTTON));
    buttonClose->setCallback([this](){
        this->setVisible(false);
    });
}

WidgetTime::~WidgetTime(){}

void WidgetTime::update(NVGcontext *ctx){
    // Get system time
    auto systemClock = std::chrono::system_clock::now();
    std::time_t systemTime = std::chrono::system_clock::to_time_t(systemClock);

    // Create UTC time string
    char strBuf[64];
    std::tm* t = std::gmtime(&systemTime);
    sprintf(&strBuf[0], "%d-%02d-%02d   %02d:%02d:%02d", 1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    if(labelUTC){
        labelUTC->setCaption(strBuf);
    }

    // Create UTC second of day time string
    auto duration = systemClock.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= seconds;
    double sod = double(60*(60 * t->tm_hour + t->tm_min) + t->tm_sec) + 0.000000001 * double(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
    sprintf(&strBuf[0], "%.9lf s", sod);
    if(labelUTC){
        labelUTCSOD->setCaption(strBuf);
    }

    // Create local time string
    std::tm* lt = std::localtime(&systemTime);
    sprintf(&strBuf[0], "%d-%02d-%02d   %02d:%02d:%02d   DST=%d", 1900 + lt->tm_year, 1 + lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, lt->tm_isdst);
    if(labelLocal){
        labelLocal->setCaption(strBuf);
    }

    // Create unix time string
    double unix = 0.001 * double(std::chrono::duration_cast<std::chrono::milliseconds>(systemClock.time_since_epoch()).count());
    sprintf(&strBuf[0], "%.03lf s",unix);
    if(labelUNIX){
        labelUNIX->setCaption(strBuf);
    }
    (void)ctx;
}

