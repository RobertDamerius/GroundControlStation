#pragma once


#include <nanogui/widget.h>
#include <nanogui/window.h>


NAMESPACE_BEGIN(nanogui)


class NANOGUI_EXPORT NonmovableWindow : public Window {
    friend class Popup;
public:
    NonmovableWindow(Widget *parent, const std::string &title = "Untitled");

    /// Handle window drag events
    virtual bool mouseDragEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override;
};


NAMESPACE_END(nanogui)

