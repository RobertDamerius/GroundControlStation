#include <NonmovableWindow.hpp>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <nanogui/screen.h>
#include <nanogui/layout.h>
#include <nanogui/serializer/core.h>


NAMESPACE_BEGIN(nanogui)


NonmovableWindow::NonmovableWindow(Widget *parent, const std::string &title): Window(parent, title){}

bool NonmovableWindow::mouseDragEvent(const Vector2i &, const Vector2i &rel, int button, int /* modifiers */){
    if(mDrag && (button & (1 << GLFW_MOUSE_BUTTON_1)) != 0) {
        // mPos += rel;
        (void)rel;
        mPos = mPos.cwiseMax(Vector2i::Zero());
        mPos = mPos.cwiseMin(parent()->size() - mSize);
        return true;
    }
    return false;
}


NAMESPACE_END(nanogui)

