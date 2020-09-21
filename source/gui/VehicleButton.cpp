#include <VehicleButton.hpp>
#include <GUI.hpp>
#include <VehicleManager.hpp>
#include <AppWindow.hpp>


#define VEHICLE_BUTTON_BORDER_RADIUS     (3)


VehicleButton::VehicleButton(nanogui::Widget *parent, nanogui::Widget *screen, const VehicleID id) : nanogui::Widget(parent), id(id), mPushed(false){
    mCaptionName = appWindow.canvas.scene.vehicleManager.GetVehicleName(this->id);
    mCaptionDescription = appWindow.canvas.scene.vehicleManager.GetVehicleDescription(this->id);
    this->setFixedWidth(222);
    this->widgetVehicle = new WidgetVehicle(screen, this->id);
    this->widgetVehicle->setVisible(false);
    this->mCallback = ([this](){
        this->widgetVehicle->setVisible(true);
        this->widgetVehicle->requestFocus();
    });
}

Eigen::Vector2i VehicleButton::preferredSize(NVGcontext *ctx)const{
    using namespace nanogui;
    nvgFontSize(ctx, GUI_FONT_SIZE);
    nvgFontFace(ctx, "sans-bold");
    float twName = nvgTextBounds(ctx, 0, 0, mCaptionName.c_str(), nullptr, nullptr);
    nvgFontFace(ctx, "sans");
    float twIP = nvgTextBounds(ctx, 0, 0, mCaptionDescription.c_str(), nullptr, nullptr);
    float tw = (twName > twIP) ? twName : twIP;
    return Eigen::Vector2i((int)tw + 20, GUI_FONT_SIZE + GUI_FONT_SIZE + 10);
}

bool VehicleButton::mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers){
    using namespace nanogui;
    Widget::mouseButtonEvent(p, button, down, modifiers);
    // Temporarily increase the reference count of the button in case the button causes the parent window to be destructed
    ref<VehicleButton> self = this;
    if(button == GLFW_MOUSE_BUTTON_1 && mEnabled){
        if(down){
            mPushed = true;
        }
        else if(mPushed){
            if(contains(p) && mCallback)
                mCallback();
            mPushed = false;
        }
        return true;
    }
    return false;
}

void VehicleButton::draw(NVGcontext *ctx){
    using namespace nanogui;

    // Button color
    NVGcolor gradTop, gradBot;
    gradTop.r = 0.0f;
    gradTop.g = 0.42f;
    gradTop.b = 0.64f;
    gradTop.a = 1.0f;
    gradBot.r = 0.0f;
    gradBot.g = 0.30f;
    gradBot.b = 0.46f;
    gradBot.a = 1.0f;
    if(mPushed){
        gradTop.g = 0.30f;
        gradTop.b = 0.46f;
        gradBot.g = 0.42f;
        gradBot.b = 0.64f;
    }
    // else if(mMouseFocus && mEnabled){
        // gradTop.g = 0.50f;
        // gradTop.b = 0.76f;
        // gradBot.g = 0.42f;
        // gradBot.b = 0.64f;
    // }

    // Button shape
    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPos.x() + 1, mPos.y() + 1.0f, mSize.x() - 2, mSize.y() - 2, VEHICLE_BUTTON_BORDER_RADIUS - 1);
    NVGpaint bg = nvgLinearGradient(ctx, mPos.x(), mPos.y(), mPos.x(), mPos.y() + mSize.y(), gradTop, gradBot);
    nvgFillPaint(ctx, bg);
    nvgFill(ctx);

    nvgBeginPath(ctx);
    nvgStrokeWidth(ctx, 1.0f);
    nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + (mPushed ? 0.5f : 1.5f), mSize.x() - 1, mSize.y() - 1 - (mPushed ? 0.0f : 1.0f), VEHICLE_BUTTON_BORDER_RADIUS);
    nvgStrokeColor(ctx, mTheme->mBorderLight);
    nvgStroke(ctx);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + 0.5f, mSize.x() - 1, mSize.y() - 2, VEHICLE_BUTTON_BORDER_RADIUS);
    nvgStrokeColor(ctx, mTheme->mBorderDark);
    nvgStroke(ctx);

    // Text: Name
    Vector2f textPos(mPos.x() + 8.0f, mPos.y() + 4.0f);
    NVGcolor textColor = mTheme->mDisabledTextColor;
    if(mEnabled){
        textColor.r = textColor.g = textColor.b = textColor.a = 1.0f;
    }
    nvgFontSize(ctx, GUI_FONT_SIZE);
    nvgFontFace(ctx, "sans-bold");
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgFillColor(ctx, mTheme->mTextColorShadow);
    nvgText(ctx, textPos.x(), textPos.y(), mCaptionName.c_str(), nullptr);
    nvgFillColor(ctx, textColor);
    nvgText(ctx, textPos.x(), textPos.y() + 1, mCaptionName.c_str(), nullptr);

    // Text: IP
    textPos = Vector2f(mPos.x() + 8.0f, mPos.y() + 4.0f + float(GUI_FONT_SIZE));
    if(mEnabled){
        textColor.a = 0.627451f;
    }
    nvgFontSize(ctx, GUI_FONT_SIZE);
    nvgFontFace(ctx, "sans");
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgFillColor(ctx, mTheme->mTextColorShadow);
    nvgText(ctx, textPos.x(), textPos.y(), mCaptionDescription.c_str(), nullptr);
    nvgFillColor(ctx, textColor);
    nvgText(ctx, textPos.x(), textPos.y() + 1, mCaptionDescription.c_str(), nullptr);

    Widget::draw(ctx);
}

