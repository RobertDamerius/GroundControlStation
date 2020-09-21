#include <GUI.hpp>
#include <Renderer.hpp>
#include <AppWindow.hpp>


GUI::GUI(){
    screen = nullptr;
    widgetToolbar = nullptr;
    widgetView = nullptr;
    widgetVehicleList = nullptr;
    widgetTime = nullptr;
}

GUI::~GUI(){}

bool GUI::Initialize(GLFWwindow* wnd, Renderer& renderer){
    // Make sure that the GUI is not initialized
    renderer.BindGUIFramebuffer();
    Terminate();

    // Initialize the screen
    screen = new nanogui::Screen();
    screen->initialize(wnd, false);

    // Create widgets
    widgetToolbar = new WidgetToolbar(screen);
    widgetView = new WidgetView(screen);
    widgetVehicleList = new WidgetVehicleList(screen);
    widgetTime = new WidgetTime(screen);
    widgetLog = new WidgetLog(screen);
    widgetInfo = new WidgetInfo(screen);
    screen->setVisible(true);
    screen->performLayout();

    // Add startup logs
    mtxStartupLogs.lock();
    for(auto&& log : startupLogs){
        widgetLog->AddLogEntry(log);
        if(widgetToolbar){
            widgetToolbar->SetLogNotification();
        }
    }
    startupLogs.clear();
    mtxStartupLogs.unlock();
    return true;
}

void GUI::Terminate(void){
    // Deleting the screen also deletes all child widgets
    if(screen){
        delete screen;
        screen = nullptr;
    }
    widgetToolbar = nullptr;
    widgetView = nullptr;
    widgetVehicleList = nullptr;
    widgetTime = nullptr;
    widgetLog = nullptr;
    widgetInfo = nullptr;
}

void GUI::Render(void){
    if(screen){
        screen->update();
        screen->performLayout();
        screen->drawContents();
        screen->drawWidgets();
    }
}

bool GUI::CursorOverGUI(glm::dvec2 cursor){
    int32_t x = (int32_t)cursor.x;
    int32_t y = (int32_t)cursor.y;
    if(screen){
        std::vector<nanogui::Widget*> childs = screen->children();
        for(auto&& c : childs){
            Eigen::Vector2i p = c->absolutePosition();
            Eigen::Vector2i d = c->size();
            if(c->visible() && (x >= p[0]) && (y >= p[1]) && (x <= (p[0] + d[0])) && (y <= (p[1] + d[1]))){
                return true;
            }
        }
    }
    return false;
}

bool GUI::IsFocused(void){
    if(screen){
        std::vector<nanogui::Widget*> childs = screen->children();
        for(auto&& c : childs){
            if(c->focused()){
                return true;
            }
        }
    }
    return false;
}

void GUI::SetWidgetViewState(bool visible){
    if(widgetView){
        widgetView->setVisible(visible);
        if(visible){
            widgetView->requestFocus();
        }
    }
}

void GUI::SetWidgetVehicleListState(bool visible){
    if(widgetVehicleList){
        widgetVehicleList->setVisible(visible);
        if(visible){
            widgetVehicleList->requestFocus();
        }
    }
}

void GUI::SetWidgetTimeState(bool visible){
    if(widgetTime){
        widgetTime->setVisible(visible);
        if(visible){
            widgetTime->requestFocus();
        }
    }
}

void GUI::SetWidgetLogState(bool visible){
    if(widgetLog){
        widgetLog->setVisible(visible);
        if(visible){
            widgetLog->requestFocus();
        }
    }
}

void GUI::SetWidgetInfoState(bool visible){
    if(widgetInfo){
        widgetInfo->setVisible(visible);
        if(visible){
            widgetInfo->requestFocus();
        }
    }
}

void GUI::AddLogEntry(LogEntry log){
    mtxStartupLogs.lock();
    if(widgetLog){
        widgetLog->AddLogEntry(log);
        if(widgetToolbar){
            widgetToolbar->SetLogNotification();
        }
    }
    else{
        startupLogs.push_back(log);
    }
    mtxStartupLogs.unlock();
}

void GUI::CallbackFramebufferResize(GLFWwindow* wnd, glm::ivec2 size){
    if(screen){
        screen->resizeCallbackEvent(size.x, size.y);
    }
    (void)wnd;
}

void GUI::CallbackKey(GLFWwindow* wnd, int key, int scancode, int action, int mods){
    if(screen){
        screen->keyCallbackEvent(key, scancode, action, mods);
    }
    (void)wnd;
}

void GUI::CallbackChar(GLFWwindow* wnd, uint32_t codepoint){
    if(screen){
        screen->charCallbackEvent(codepoint);
    }
    (void)wnd;
}

void GUI::CallbackMouseButton(GLFWwindow* wnd, int button, int action, int mods){
    if(screen){
        screen->mouseButtonCallbackEvent(button, action, mods);
    }
    (void)wnd;
}

void GUI::CallbackCursorPosition(GLFWwindow* wnd, glm::dvec2 cursor, glm::dvec2 deltaCursor){
    if(screen){
        screen->cursorPosCallbackEvent(cursor.x, cursor.y);
    }
    (void)wnd;
    (void)deltaCursor;
}

void GUI::CallbackScroll(GLFWwindow* wnd, glm::dvec2 scroll){
    if(screen){
        screen->scrollCallbackEvent(scroll.x, scroll.y);
    }
    (void)wnd;
}

void GUI::CallbackDrop(GLFWwindow* wnd, int count, const char **filenames){
    if(screen){
        screen->dropCallbackEvent(count, filenames);
    }
    (void)wnd;
}

