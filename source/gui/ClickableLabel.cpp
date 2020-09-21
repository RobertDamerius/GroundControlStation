#include <ClickableLabel.hpp>


ClickableLabel::ClickableLabel(nanogui::Widget* parent, const std::string &caption, const std::string &font, int fontSize): nanogui::Button(parent, caption), mFont(font){
    nanogui::Button::setFontSize(fontSize);
}

ClickableLabel::~ClickableLabel(){}

void ClickableLabel::setFont(const std::string &font){
    mFont = font;
}

void ClickableLabel::draw(NVGcontext *ctx) {
    using namespace nanogui;
    Widget::draw(ctx);

    int fontSize = mFontSize == -1 ? mTheme->mButtonFontSize : mFontSize;
    nvgFontSize(ctx, fontSize);
    nvgFontFace(ctx, mFont.c_str());
    float tw = nvgTextBounds(ctx, 0,0, mCaption.c_str(), nullptr, nullptr);
    Vector2f center = mPos.cast<float>() + mSize.cast<float>() * 0.5f;
    Vector2f textPos(center.x() - tw * 0.5f, center.y() - 1);
    NVGcolor textColor = mTextColor.w() == 0 ? mTheme->mTextColor : mTextColor;
    if (!mEnabled)
        textColor = mTheme->mDisabledTextColor;
    nvgFontSize(ctx, fontSize);
    nvgFontFace(ctx, mFont.c_str());
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(ctx, mTheme->mTextColorShadow);
    nvgText(ctx, textPos.x(), textPos.y(), mCaption.c_str(), nullptr);
    nvgFillColor(ctx, textColor);
    nvgText(ctx, textPos.x(), textPos.y() + 1, mCaption.c_str(), nullptr);
}

