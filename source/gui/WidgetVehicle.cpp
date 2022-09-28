#include <WidgetVehicle.hpp>
#include <AppWindow.hpp>
#include <Configuration.hpp>
#include <GUI.hpp>


#define DATABLOCK_MARGIN                  (8)
#define TAB_SETTINGS_WIDTH_COLUMN_LABEL   (110)
#define TAB_SETTINGS_WIDTH_ITEM           (100)
#define TAB_SETTINGS_HEIGHT_ITEM          (20)


WidgetVehicle::WidgetVehicle(nanogui::Widget *parent, const VehicleID id): nanogui::Window(parent, appWindow.canvas.scene.vehicleManager.GetVehicleName(id)), id(id){
    using namespace nanogui;
    this->setPosition(Vector2i(10, 48));
    this->setLayout(new GroupLayout());
    TabWidget* tabWidget = this->add<TabWidget>();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Tab: DATA
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Widget* layer = tabWidget->createTab("DATA");
    layer->setLayout(new GroupLayout());
    Widget *gridContent = new Widget(layer);
    gridContent->setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Minimum, 0, 8));
    Widget *dataBlock, *content;

    // Time
    dataBlock = new Widget(gridContent);
    dataBlock->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));
    new Label(dataBlock, "Time to latest message", "sans-bold", GUI_FONT_SIZE);
    content = new Widget(dataBlock);
    content->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill, DATABLOCK_MARGIN, 0));
    timeNavigation = new ValueElement(content, "Navigation", "s");
    timeGuidance = new ValueElement(content, "Guidance", "s", 4, {GUI_TEXT_COLOR_GUIDANCE_R,GUI_TEXT_COLOR_GUIDANCE_G,GUI_TEXT_COLOR_GUIDANCE_B,255});
    timeGuidanceTrajectory = new ValueElement(content, "Trajectory", "s");
    timeGuidanceWaypoint = new ValueElement(content, "Waypoint", "s");
    timePolygon = new ValueElement(content, "Polygon", "s");

    // Position (LLA)
    dataBlock = new Widget(gridContent);
    dataBlock->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));
    new Label(dataBlock, "Position (LLA)", "sans-bold", GUI_FONT_SIZE);
    content = new Widget(dataBlock);
    content->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill, DATABLOCK_MARGIN, 0));
    latitude = new ValueNavigationGuidance(content, "Latitude", "deg");
    longitude = new ValueNavigationGuidance(content, "Longitude", "deg");
    altitude = new ValueNavigationGuidance(content, "Altitude", "m");

    // Velocity (NED)
    dataBlock = new Widget(gridContent);
    dataBlock->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));
    new Label(dataBlock, "Velocity (NED)", "sans-bold", GUI_FONT_SIZE);
    content = new Widget(dataBlock);
    content->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill, DATABLOCK_MARGIN, 0));
    velocityNorth = new ValueNavigationGuidance(content, "North (x)", "m/s");
    velocityEast = new ValueNavigationGuidance(content, "East (y)", "m/s");
    velocityDown = new ValueNavigationGuidance(content, "Down (z)", "m/s");

    // Position (NED)
    dataBlock = new Widget(gridContent);
    dataBlock->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));
    new Label(dataBlock, "Position (XYZ)", "sans-bold", GUI_FONT_SIZE);
    content = new Widget(dataBlock);
    content->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill, DATABLOCK_MARGIN, 0));
    positionX = new ValueNavigationGuidance(content, "x", "m");
    positionY = new ValueNavigationGuidance(content, "y", "m");
    positionZ = new ValueNavigationGuidance(content, "z", "m");

    // Velocity (UVW)
    dataBlock = new Widget(gridContent);
    dataBlock->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));
    new Label(dataBlock, "Body Velocity (Translation)", "sans-bold", GUI_FONT_SIZE);
    content = new Widget(dataBlock);
    content->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill, DATABLOCK_MARGIN, 0));
    velocityU = new ValueNavigationGuidance(content, "u", "m/s");
    velocityV = new ValueNavigationGuidance(content, "v", "m/s");
    velocityW = new ValueNavigationGuidance(content, "w", "m/s");

    // Orientation
    dataBlock = new Widget(gridContent);
    dataBlock->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));
    new Label(dataBlock, "Orientation", "sans-bold", GUI_FONT_SIZE);
    content = new Widget(dataBlock);
    content->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill, DATABLOCK_MARGIN, 0));
    roll = new ValueNavigationGuidance(content, "Roll", "deg");
    pitch = new ValueNavigationGuidance(content, "Pitch", "deg");
    yaw = new ValueNavigationGuidance(content, "Yaw", "deg");

    // Velocity (PQR)
    dataBlock = new Widget(gridContent);
    dataBlock->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));
    new Label(dataBlock, "Body Velocity (Rotation)", "sans-bold", GUI_FONT_SIZE);
    content = new Widget(dataBlock);
    content->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill, DATABLOCK_MARGIN, 0));
    velocityP = new ValueNavigationGuidance(content, "p", "deg/s");
    velocityQ = new ValueNavigationGuidance(content, "q", "deg/s");
    velocityR = new ValueNavigationGuidance(content, "r", "deg/s");

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Tab: NAV
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // layer = tabWidget->createTab("NAV");
    // layer->setLayout(new GroupLayout());
    // layer->add<Label>("Function graph widget", "sans-bold");
    // Graph *graph = layer->add<Graph>("Some Function");
    // graph->setHeader("E = 2.35e-3");
    // graph->setFooter("Iteration 89");
    // VectorXf &func = graph->values();
    // func.resize(100);
    // for (int i = 0; i < 100; ++i)
        // func[i] = 0.5f * (0.5f * std::sin(i / 10.f) + 0.5f * std::cos(i / 23.f) + 1);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Tab: SETTINGS
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    layer = tabWidget->createTab("SETTINGS");
    layer->setLayout(new GroupLayout());
    gridContent = new Widget(layer);
    gridContent->setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Fill));
    Widget* contentLeft = new Widget(gridContent);
    Widget* contentRight = new Widget(gridContent);
    contentLeft->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum));
    contentRight->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum));

    // Group: Position history
    {
        new Label(contentLeft, "Position History", "sans-bold", GUI_FONT_SIZE);
        Widget* group = new Widget(contentLeft);
        group->setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, 8, 4));
        new Label(group, "Enable", "sans", GUI_FONT_SIZE);
            CheckBox* cbEnable = new CheckBox(group, "");
        new Label(group, "Maximum SOG", "sans", GUI_FONT_SIZE);
            TextBox* textBoxVelocity = new TextBox(group);
            textBoxVelocity->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxVelocity->setUnits("m/s");
            textBoxVelocity->setFontSize(GUI_FONT_SIZE);
            textBoxVelocity->setFormat("[0-9]*\\.?[0-9]+");
            textBoxVelocity->setCallback([this, textBoxVelocity](const std::string& str){
                double d = 0.0;
                try{ d = std::stod(str); } catch(...){}
                if(d > 0.0){
                    Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                    if(v){
                        v->positionHistory.SetMaximumSOG(d);
                        d = v->positionHistory.GetMaximumSOG();
                        std::stringstream stream;
                        stream << std::fixed << std::setprecision(1) << d;
                        textBoxVelocity->setValue(stream.str());
                        appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    }
                }
                return d > 0.0;
            });
        new Label(group, "Line Width", "sans", GUI_FONT_SIZE);
            TextBox* textBoxLineWidth = new TextBox(group);
            textBoxLineWidth->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxLineWidth->setUnits("px");
            textBoxLineWidth->setFontSize(GUI_FONT_SIZE);
            textBoxLineWidth->setFormat("[0-9]*\\.?[0-9]+");
            textBoxLineWidth->setCallback([this](const std::string& str){
                double d = 0.0;
                try{ d = std::stod(str); } catch(...){}
                if(d > 0.0){
                    Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                    if(v){
                        v->positionHistory.width = (GLfloat)d;
                        appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    }
                }
                return d > 0.0;
            });
        new Label(group, "Update Period", "sans", GUI_FONT_SIZE);
            TextBox* textBoxUpdatePeriod = new TextBox(group);
            textBoxUpdatePeriod->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxUpdatePeriod->setUnits("ms");
            textBoxUpdatePeriod->setFontSize(GUI_FONT_SIZE);
            textBoxUpdatePeriod->setFormat("[0-9]+");
            textBoxUpdatePeriod->setCallback([this](const std::string& str){
                int i = 0;
                try{ i = std::stoi(str); } catch(...){}
                if(i > 0){
                    Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                    if(v){
                        v->positionHistory.SetTimePeriodMs((uint32_t)i);
                        appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    }
                }
                return i > 0;
            });
        new Label(group, "Buffer Size", "sans", GUI_FONT_SIZE);
            TextBox* textBoxBufferSize = new TextBox(group);
            textBoxBufferSize->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxBufferSize->setFontSize(GUI_FONT_SIZE);
            textBoxBufferSize->setFormat("[0-9]+");
            textBoxBufferSize->setCallback([this, textBoxBufferSize](const std::string& str){
                int i = 0;
                try{ i = std::stoi(str); } catch(...){}
                if(i > 0){
                    Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                    if(v){
                        v->positionHistory.Resize((uint32_t)i);
                        textBoxBufferSize->setValue(std::to_string(v->positionHistory.GetBufferSize()));
                        appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    }
                }
                return i > 0;
            });
        cbEnable->setCallback([this, textBoxVelocity, textBoxLineWidth, textBoxUpdatePeriod, textBoxBufferSize](bool checked){
            textBoxVelocity->setEnabled(checked);
            textBoxVelocity->setEditable(checked);
            textBoxLineWidth->setEnabled(checked);
            textBoxLineWidth->setEditable(checked);
            textBoxUpdatePeriod->setEnabled(checked);
            textBoxUpdatePeriod->setEditable(checked);
            textBoxBufferSize->setEnabled(checked);
            textBoxBufferSize->setEditable(checked);
            Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
            if(v){
                v->positionHistory.enable = checked;
                appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            }
        });
        Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
        if(v){
            bool checked = v->positionHistory.enable;
            cbEnable->setChecked(checked);
            textBoxVelocity->setEnabled(checked);
            textBoxVelocity->setEditable(checked);
            textBoxLineWidth->setEnabled(checked);
            textBoxLineWidth->setEditable(checked);
            textBoxUpdatePeriod->setEnabled(checked);
            textBoxUpdatePeriod->setEditable(checked);
            textBoxBufferSize->setEnabled(checked);
            textBoxBufferSize->setEditable(checked);
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(1) << v->positionHistory.width;
                textBoxLineWidth->setValue(stream.str());
            }
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(1) << v->positionHistory.GetMaximumSOG();
                textBoxVelocity->setValue(stream.str());
            }
            uint32_t u = v->positionHistory.GetBufferSize();
            textBoxBufferSize->setValue(std::to_string(u ? u : Configuration::style.positionHistoryBufferSize));
            textBoxUpdatePeriod->setValue(std::to_string(v->positionHistory.GetTimePeriodMs()));
            appWindow.canvas.scene.vehicleManager.UnlockVehicle();
        }
        Widget* w = new Widget(group);
        w->setFixedWidth(TAB_SETTINGS_WIDTH_COLUMN_LABEL);
    }

    // Group: View
    {
        new Label(contentRight, "View", "sans-bold", GUI_FONT_SIZE);
        Widget* group = new Widget(contentRight);
        group->setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, 8, 4));
        new Label(group, "Visible", "sans", GUI_FONT_SIZE);
            CheckBox* cbView = new CheckBox(group, "");
            cbView->setCallback([this](bool checked){
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    v->visible = checked;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
        new Label(group, "Use Custom Mesh", "sans", GUI_FONT_SIZE);
            CheckBox* cbUseMesh = new CheckBox(group, "");
            cbUseMesh->setCallback([this](bool checked){
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    v->useMesh = checked;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
        new Label(group, "Altitude To Zero", "sans", GUI_FONT_SIZE);
            CheckBox* cbAltZero = new CheckBox(group, "");
            cbAltZero->setCallback([this](bool checked){
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    v->altitudeToZero = checked;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
        new Label(group, "Lock Camera", "sans", GUI_FONT_SIZE);
            Button* buttonLock = new Button(group, "Translation", ENTYPO_ICON_LOCK);
            buttonLock->setFontSize(GUI_FONT_SIZE);
            buttonLock->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            buttonLock->setCallback([this](){
                appWindow.canvas.scene.humanInterface.cameraLock.lock = true;
                appWindow.canvas.scene.humanInterface.cameraLock.lockRotation = false;
                appWindow.canvas.scene.humanInterface.cameraLock.id = this->id;
            });
        Widget* w = new Widget(group);
        w->setFixedWidth(TAB_SETTINGS_WIDTH_COLUMN_LABEL);
            Button* buttonLockR = new Button(group, "Rotation", ENTYPO_ICON_LOCK);
            buttonLockR->setFontSize(GUI_FONT_SIZE);
            buttonLockR->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            buttonLockR->setCallback([this](){
                appWindow.canvas.scene.humanInterface.cameraLock.lock = true;
                appWindow.canvas.scene.humanInterface.cameraLock.lockRotation = true;
                appWindow.canvas.scene.humanInterface.cameraLock.id = this->id;
            });
        w = new Widget(group);
        w->setFixedWidth(TAB_SETTINGS_WIDTH_COLUMN_LABEL);
        Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
        if(v){
            cbView->setChecked(v->visible);
            cbUseMesh->setChecked(v->useMesh);
            cbAltZero->setChecked(v->altitudeToZero);
            appWindow.canvas.scene.vehicleManager.UnlockVehicle();
        }
    }

    // Group: Compass
    {
        new Label(contentRight, "Compass", "sans-bold", GUI_FONT_SIZE);
        Widget* group = new Widget(contentRight);
        group->setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, 8, 4));
        new Label(group, "Enable", "sans", GUI_FONT_SIZE);
            CheckBox* cbEnable = new CheckBox(group, "");
        new Label(group, "Radius", "sans", GUI_FONT_SIZE);
            TextBox* textBoxRadius = new TextBox(group);
            textBoxRadius->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxRadius->setUnits("m");
            textBoxRadius->setFontSize(GUI_FONT_SIZE);
            textBoxRadius->setFormat("[0-9]*\\.?[0-9]+");
            textBoxRadius->setCallback([this, textBoxRadius](const std::string& str){
                double d = 0.0;
                try{ d = std::stod(str); } catch(...){}
                if(d > 0.0){
                    Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                    if(v){
                        d = v->compass.Resize(d);
                        appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                        std::stringstream stream;
                        stream << std::fixed << std::setprecision(1) << d;
                        textBoxRadius->setValue(stream.str());
                    }
                }
                return d > 0.0;
            });
        new Label(group, "Color", "sans", GUI_FONT_SIZE);
            ColorPicker* cp = new ColorPicker(group);
            cp->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
            if(v){
                cp->setColor(Color((float)sqrt(v->compass.color.x), (float)sqrt(v->compass.color.y), (float)sqrt(v->compass.color.z), 1.0f));
                appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            }
            cp->setCallback([this](const Color& c){
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->compass.color = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
            cp->setFinalCallback([this](const Color &c) {
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->compass.color = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
        cbEnable->setCallback([this, textBoxRadius, cp](bool checked){
            textBoxRadius->setEnabled(checked);
            textBoxRadius->setEditable(checked);
            cp->setEnabled(checked);
            Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
            if(v){
                v->compass.enable = checked;
                appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            }
        });
        v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
        if(v){
            bool checked = v->compass.enable;
            cbEnable->setChecked(checked);
            textBoxRadius->setEnabled(checked);
            textBoxRadius->setEditable(checked);
            cp->setEnabled(checked);
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(1) << v->compass.GetRadius();
                textBoxRadius->setValue(stream.str());
            }
            appWindow.canvas.scene.vehicleManager.UnlockVehicle();
        }
        Widget* w = new Widget(group);
        w->setFixedWidth(TAB_SETTINGS_WIDTH_COLUMN_LABEL);
    }

    // Group: Waypoints
    {
        new Label(contentLeft, "Waypoints", "sans-bold", GUI_FONT_SIZE);
        Widget* group = new Widget(contentLeft);
        group->setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, 8, 4));
        new Label(group, "Enable", "sans", GUI_FONT_SIZE);
            CheckBox* cbEnable = new CheckBox(group, "");
            cbEnable->setChecked(true);
        new Label(group, "Line Color", "sans", GUI_FONT_SIZE);
            ColorPicker* cp = new ColorPicker(group);
            cp->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
            if(v){
                cp->setColor(Color((float)sqrt(v->waypoints.color.x), (float)sqrt(v->waypoints.color.y), (float)sqrt(v->waypoints.color.z), 1.0f));
                appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            }
            cp->setCallback([this](const Color& c){
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->waypoints.color = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
            cp->setFinalCallback([this](const Color &c) {
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->waypoints.color = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
        new Label(group, "Line Width", "sans", GUI_FONT_SIZE);
            TextBox* textBoxLineWidth = new TextBox(group);
            textBoxLineWidth->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxLineWidth->setUnits("px");
            textBoxLineWidth->setFontSize(GUI_FONT_SIZE);
            textBoxLineWidth->setFormat("[0-9]*\\.?[0-9]+");
            textBoxLineWidth->setCallback([this, textBoxLineWidth](const std::string& str){
                double d = 0.0;
                try{ d = std::stod(str); } catch(...){}
                if(d > 0.0){
                    Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                    if(v){
                        v->waypoints.SetLineWidth(d);
                        d = v->waypoints.GetLineWidth();
                        std::stringstream stream;
                        stream << std::fixed << std::setprecision(1) << d;
                        textBoxLineWidth->setValue(stream.str());
                        appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    }
                }
                return d > 0.0;
            });
        new Label(group, "Symbol Size", "sans", GUI_FONT_SIZE);
            TextBox* textSymbolSize = new TextBox(group);
            textSymbolSize->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textSymbolSize->setUnits("m");
            textSymbolSize->setFontSize(GUI_FONT_SIZE);
            textSymbolSize->setFormat("[0-9]*\\.?[0-9]+");
            textSymbolSize->setCallback([this, textSymbolSize](const std::string& str){
                double d = 0.0;
                try{ d = std::stod(str); } catch(...){}
                if(d > 0.0){
                    Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                    if(v){
                        v->waypoints.SetSymbolSize(d);
                        d = v->waypoints.GetSymbolSize();
                        v->waypoints.shouldGenerate = true;
                        std::stringstream stream;
                        stream << std::fixed << std::setprecision(1) << d;
                        textSymbolSize->setValue(stream.str());
                        appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    }
                }
                return d > 0.0;
            });
        new Label(group, "Vehicle Color", "sans", GUI_FONT_SIZE);
            ColorPicker* cpV = new ColorPicker(group);
            cpV->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
            if(v){
                cpV->setColor(Color((float)sqrt(v->waypoints.vehicleColor.x), (float)sqrt(v->waypoints.vehicleColor.y), (float)sqrt(v->waypoints.vehicleColor.z), 1.0f));
                appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            }
            cpV->setCallback([this](const Color& c){
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->waypoints.vehicleColor = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
            cpV->setFinalCallback([this](const Color &c) {
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->waypoints.vehicleColor = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
        new Label(group, "Vehicle Alpha", "sans", GUI_FONT_SIZE);
            TextBox* textBoxVehicleAlpha = new TextBox(group);
            textBoxVehicleAlpha->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxVehicleAlpha->setFontSize(GUI_FONT_SIZE);
            textBoxVehicleAlpha->setFormat("[0-9]*\\.?[0-9]+");
            textBoxVehicleAlpha->setCallback([this, textBoxVehicleAlpha](const std::string& str){
                double d = 0.0;
                try{ d = std::stod(str); } catch(...){}
                if(d >= 0.0){
                    Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                    if(v){
                        v->waypoints.SetVehicleAlpha(d);
                        d = v->waypoints.GetVehicleAlpha();
                        std::stringstream stream;
                        stream << std::fixed << std::setprecision(2) << d;
                        textBoxVehicleAlpha->setValue(stream.str());
                        appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    }
                }
                return d >= 0.0;
            });
        cbEnable->setCallback([this, cp, cpV, textBoxLineWidth, textSymbolSize, textBoxVehicleAlpha](bool checked){
            cp->setEnabled(checked);
            cpV->setEnabled(checked);
            textBoxLineWidth->setEnabled(checked);
            textBoxLineWidth->setEditable(checked);
            textBoxVehicleAlpha->setEnabled(checked);
            textBoxVehicleAlpha->setEditable(checked);
            textSymbolSize->setEnabled(checked);
            textSymbolSize->setEditable(checked);
            Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
            if(v){
                v->waypoints.enable = checked;
                appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            }
        });
        v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
        if(v){
            bool checked = v->waypoints.enable;
            cbEnable->setChecked(checked);
            textBoxLineWidth->setEnabled(checked);
            textBoxLineWidth->setEditable(checked);
            textBoxVehicleAlpha->setEnabled(checked);
            textBoxVehicleAlpha->setEditable(checked);
            textSymbolSize->setEnabled(checked);
            textSymbolSize->setEditable(checked);
            cp->setEnabled(checked);
            cpV->setEnabled(checked);
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(1) << v->waypoints.GetLineWidth();
                textBoxLineWidth->setValue(stream.str());
            }
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << v->waypoints.GetVehicleAlpha();
                textBoxVehicleAlpha->setValue(stream.str());
            }
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(1) << v->waypoints.GetSymbolSize();
                textSymbolSize->setValue(stream.str());
            }
            appWindow.canvas.scene.vehicleManager.UnlockVehicle();
        }
        Widget* w = new Widget(group);
        w->setFixedWidth(TAB_SETTINGS_WIDTH_COLUMN_LABEL);
    }

    // Group: Trajectory
    {
        new Label(contentLeft, "Trajectory", "sans-bold", GUI_FONT_SIZE);
        Widget* group = new Widget(contentLeft);
        group->setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, 8, 4));
        new Label(group, "Enable", "sans", GUI_FONT_SIZE);
            CheckBox* cbEnable = new CheckBox(group, "");
            cbEnable->setChecked(true);
        new Label(group, "Color", "sans", GUI_FONT_SIZE);
            ColorPicker* cp = new ColorPicker(group);
            cp->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
            if(v){
                cp->setColor(Color((float)sqrt(v->trajectory.color.x), (float)sqrt(v->trajectory.color.y), (float)sqrt(v->trajectory.color.z), 1.0f));
                appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            }
            cp->setCallback([this](const Color& c){
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->trajectory.color = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
            cp->setFinalCallback([this](const Color &c) {
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->trajectory.color = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
        new Label(group, "Line Width", "sans", GUI_FONT_SIZE);
            TextBox* textBoxLineWidth = new TextBox(group);
            textBoxLineWidth->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxLineWidth->setUnits("px");
            textBoxLineWidth->setFontSize(GUI_FONT_SIZE);
            textBoxLineWidth->setFormat("[0-9]*\\.?[0-9]+");
            textBoxLineWidth->setCallback([this, textBoxLineWidth](const std::string& str){
                double d = 0.0;
                try{ d = std::stod(str); } catch(...){}
                if(d > 0.0){
                    Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                    if(v){
                        v->trajectory.SetLineWidth(d);
                        d = v->trajectory.GetLineWidth();
                        std::stringstream stream;
                        stream << std::fixed << std::setprecision(1) << d;
                        textBoxLineWidth->setValue(stream.str());
                        appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    }
                }
                return d > 0.0;
            });
        new Label(group, "Vehicle Color", "sans", GUI_FONT_SIZE);
            ColorPicker* cpV = new ColorPicker(group);
            cpV->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
            if(v){
                cpV->setColor(Color((float)sqrt(v->trajectory.vehicleColor.x), (float)sqrt(v->trajectory.vehicleColor.y), (float)sqrt(v->trajectory.vehicleColor.z), 1.0f));
                appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            }
            cpV->setCallback([this](const Color& c){
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->trajectory.vehicleColor = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
            cpV->setFinalCallback([this](const Color &c) {
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->trajectory.vehicleColor = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
        new Label(group, "Vehicle Alpha", "sans", GUI_FONT_SIZE);
            TextBox* textBoxVehicleAlpha = new TextBox(group);
            textBoxVehicleAlpha->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxVehicleAlpha->setFontSize(GUI_FONT_SIZE);
            textBoxVehicleAlpha->setFormat("[0-9]*\\.?[0-9]+");
            textBoxVehicleAlpha->setCallback([this, textBoxVehicleAlpha](const std::string& str){
                double d = 0.0;
                try{ d = std::stod(str); } catch(...){}
                if(d >= 0.0){
                    Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                    if(v){
                        v->trajectory.SetVehicleAlpha(d);
                        d = v->trajectory.GetVehicleAlpha();
                        std::stringstream stream;
                        stream << std::fixed << std::setprecision(2) << d;
                        textBoxVehicleAlpha->setValue(stream.str());
                        appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    }
                }
                return d >= 0.0;
            });
        new Label(group, "Vehicle Stride", "sans", GUI_FONT_SIZE);
            TextBox* textBoxVehicleStride = new TextBox(group);
            textBoxVehicleStride->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxVehicleStride->setFontSize(GUI_FONT_SIZE);
            textBoxVehicleStride->setUnits("s");
            textBoxVehicleStride->setFormat("[0-9]*\\.?[0-9]+");
            textBoxVehicleStride->setCallback([this, textBoxVehicleStride](const std::string& str){
                double d = 0.0;
                try{ d = std::stod(str); } catch(...){}
                if(d >= 0.0){
                    Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                    if(v){
                        v->trajectory.SetVehicleStride(d);
                        d = v->trajectory.GetVehicleStride();
                        std::stringstream stream;
                        stream << std::fixed << std::setprecision(2) << d;
                        textBoxVehicleStride->setValue(stream.str());
                        appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    }
                }
                return d >= 0.0;
            });
        cbEnable->setCallback([this, cp, cpV, textBoxLineWidth, textBoxVehicleAlpha, textBoxVehicleStride](bool checked){
            cp->setEnabled(checked);
            cpV->setEnabled(checked);
            textBoxLineWidth->setEnabled(checked);
            textBoxLineWidth->setEditable(checked);
            textBoxVehicleAlpha->setEnabled(checked);
            textBoxVehicleAlpha->setEditable(checked);
            textBoxVehicleStride->setEnabled(checked);
            textBoxVehicleStride->setEditable(checked);
            Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
            if(v){
                v->trajectory.enable = checked;
                appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            }
        });
        v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
        if(v){
            bool checked = v->trajectory.enable;
            cbEnable->setChecked(checked);
            textBoxLineWidth->setEnabled(checked);
            textBoxLineWidth->setEditable(checked);
            textBoxVehicleAlpha->setEnabled(checked);
            textBoxVehicleAlpha->setEditable(checked);
            textBoxVehicleStride->setEnabled(checked);
            textBoxVehicleStride->setEditable(checked);
            cp->setEnabled(checked);
            cpV->setEnabled(checked);
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(1) << v->trajectory.GetLineWidth();
                textBoxLineWidth->setValue(stream.str());
            }
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << v->trajectory.GetVehicleAlpha();
                textBoxVehicleAlpha->setValue(stream.str());
            }
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << v->trajectory.GetVehicleStride();
                textBoxVehicleStride->setValue(stream.str());
            }
            appWindow.canvas.scene.vehicleManager.UnlockVehicle();
        }
        Widget* w = new Widget(group);
        w->setFixedWidth(TAB_SETTINGS_WIDTH_COLUMN_LABEL);
    }

    // Group: Polygons
    {
        new Label(contentRight, "Polygons", "sans-bold", GUI_FONT_SIZE);
        Widget* group = new Widget(contentRight);
        group->setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, 8, 4));
        new Label(group, "Enable", "sans", GUI_FONT_SIZE);
            CheckBox* cbEnable = new CheckBox(group, "");
            cbEnable->setChecked(true);
            cbEnable->setCallback([this](bool checked){
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    v->polygons.enable = checked;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
        new Label(group, "Upper limit", "sans", GUI_FONT_SIZE);
            TextBox* textBoxUpperLimit = new TextBox(group, "1.0");
            textBoxUpperLimit->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxUpperLimit->setUnits("m");
            textBoxUpperLimit->setFontSize(GUI_FONT_SIZE);
            textBoxUpperLimit->setFormat("[+-]?[0-9]*\\.?[0-9]+");
            textBoxUpperLimit->setEnabled(true);
            textBoxUpperLimit->setEditable(true);
            textBoxUpperLimit->setCallback([this, textBoxUpperLimit](const std::string& str){
                double value = 0.0;
                try{ value = std::stod(str); } catch(...){}
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    v->polygons.upperLimit = (GLfloat)value;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    std::stringstream stream;
                    stream << std::fixed << std::setprecision(1) << value;
                    textBoxUpperLimit->setValue(stream.str());
                }
                return true;
            });
        new Label(group, "Lower limit", "sans", GUI_FONT_SIZE);
            TextBox* textBoxLowerLimit = new TextBox(group, "-1.0");
            textBoxLowerLimit->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            textBoxLowerLimit->setUnits("m");
            textBoxLowerLimit->setFontSize(GUI_FONT_SIZE);
            textBoxLowerLimit->setFormat("[+-]?[0-9]*\\.?[0-9]+");
            textBoxLowerLimit->setEnabled(true);
            textBoxLowerLimit->setEditable(true);
            textBoxLowerLimit->setCallback([this, textBoxLowerLimit](const std::string& str){
                double value = 0.0;
                try{ value = std::stod(str); } catch(...){}
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    v->polygons.lowerLimit = (GLfloat)value;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                    std::stringstream stream;
                    stream << std::fixed << std::setprecision(1) << value;
                    textBoxLowerLimit->setValue(stream.str());
                }
                return true;
            });
        new Label(group, "Color", "sans", GUI_FONT_SIZE);
            ColorPicker* cp = new ColorPicker(group);
            cp->setFixedSize(Vector2i(TAB_SETTINGS_WIDTH_ITEM, TAB_SETTINGS_HEIGHT_ITEM));
            Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
            if(v){
                cp->setColor(Color((float)sqrt(v->polygons.color.x), (float)sqrt(v->polygons.color.y), (float)sqrt(v->polygons.color.z), 1.0f));
                appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            }
            cp->setCallback([this](const Color& c){
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->polygons.color = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
            cp->setFinalCallback([this](const Color &c) {
                Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
                if(v){
                    glm::vec3 clr(c.r()*c.r(), c.g()*c.g(), c.b()*c.b());
                    v->polygons.color = clr;
                    appWindow.canvas.scene.vehicleManager.UnlockVehicle();
                }
            });
        Widget* w = new Widget(group);
        w->setFixedWidth(TAB_SETTINGS_WIDTH_COLUMN_LABEL);
        cbEnable->setCallback([this](bool checked){
            Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
            if(v){
                v->polygons.enable = checked;
                appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            }
        });
        v = appWindow.canvas.scene.vehicleManager.LockVehicle(this->id);
        if(v){
            cbEnable->setChecked(v->polygons.enable);
            appWindow.canvas.scene.vehicleManager.UnlockVehicle();
        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Close button
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    tabWidget->setActiveTab(0);
    Button* buttonClose = new Button(this, "Close", ENTYPO_ICON_CIRCLE_WITH_CROSS);
    buttonClose->setBackgroundColor(Color(GUI_COLOR_CLOSE_BUTTON));
    buttonClose->setCallback([this](){
        this->setVisible(false);
    });
}

WidgetVehicle::~WidgetVehicle(){}

void WidgetVehicle::update(NVGcontext *ctx){
    // Vehicle name might have been changed: update window title
    setTitle(appWindow.canvas.scene.vehicleManager.GetVehicleName(this->id));

    // Get information about vehicle from vehicle manager
    RigidBodyState guidance, navigation;
    Vehicle::TimeOfLatestMessage timeOfLatestMessage;
    appWindow.canvas.scene.vehicleManager.GetVehicleState(this->id, guidance, navigation, timeOfLatestMessage);

    this->latitude->guidance = guidance.latitude.Deg();
    this->latitude->navigation = navigation.latitude.Deg();
    this->latitude->guidanceActive = (guidance.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->latitude->navigationActive = (navigation.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->longitude->guidance = guidance.longitude.Deg();
    this->longitude->navigation = navigation.longitude.Deg();
    this->longitude->guidanceActive = (guidance.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->longitude->navigationActive = (navigation.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->altitude->guidance = guidance.altitude;
    this->altitude->navigation = navigation.altitude;
    this->altitude->guidanceActive = (guidance.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (guidance.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->altitude->navigationActive = (navigation.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (navigation.conf.positionLLA.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->positionX->guidance = guidance.position.x;
    this->positionX->navigation = navigation.position.x;
    this->positionX->guidanceActive = (guidance.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->positionX->navigationActive = (navigation.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->positionY->guidance = guidance.position.y;
    this->positionY->navigation = navigation.position.y;
    this->positionY->guidanceActive = (guidance.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->positionY->navigationActive = (navigation.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->positionZ->guidance = guidance.position.z;
    this->positionZ->navigation = navigation.position.z;
    this->positionZ->guidanceActive = (guidance.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (guidance.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->positionZ->navigationActive = (navigation.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (navigation.conf.positionNED.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->roll->guidance = guidance.roll.Deg();
    this->roll->navigation = navigation.roll.Deg();
    this->roll->guidanceActive = (guidance.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->roll->navigationActive = (navigation.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->pitch->guidance = guidance.pitch.Deg();
    this->pitch->navigation = navigation.pitch.Deg();
    this->pitch->guidanceActive = (guidance.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->pitch->navigationActive = (navigation.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->yaw->guidance = guidance.yaw.Deg();
    this->yaw->navigation = navigation.yaw.Deg();
    this->yaw->guidanceActive = (guidance.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (guidance.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->yaw->navigationActive = (navigation.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (navigation.conf.orientationRollPitchYaw.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->velocityNorth->guidance = guidance.velocityNorth;
    this->velocityNorth->navigation = navigation.velocityNorth;
    this->velocityNorth->guidanceActive = (guidance.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->velocityNorth->navigationActive = (navigation.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->velocityEast->guidance = guidance.velocityEast;
    this->velocityEast->navigation = navigation.velocityEast;
    this->velocityEast->guidanceActive = (guidance.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->velocityEast->navigationActive = (navigation.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->velocityDown->guidance = guidance.velocityDown;
    this->velocityDown->navigation = navigation.velocityDown;
    this->velocityDown->guidanceActive = (guidance.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (guidance.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->velocityDown->navigationActive = (navigation.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (navigation.conf.velocityNEDUVW.availableE == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->velocityU->guidance = guidance.u;
    this->velocityU->navigation = navigation.u;
    this->velocityU->guidanceActive = (guidance.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->velocityU->navigationActive = (navigation.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->velocityV->guidance = guidance.v;
    this->velocityV->navigation = navigation.v;
    this->velocityV->guidanceActive = (guidance.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->velocityV->navigationActive = (navigation.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->velocityW->guidance = guidance.w;
    this->velocityW->navigation = navigation.w;
    this->velocityW->guidanceActive = (guidance.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (guidance.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->velocityW->navigationActive = (navigation.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (navigation.conf.velocityNEDUVW.availableB == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->velocityP->guidance = 57.29577951308232088 * guidance.p;
    this->velocityP->navigation = 57.29577951308232088 * navigation.p;
    this->velocityP->guidanceActive = (guidance.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->velocityP->navigationActive = (navigation.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->velocityQ->guidance = 57.29577951308232088 * guidance.q;
    this->velocityQ->navigation = 57.29577951308232088 * navigation.q;
    this->velocityQ->guidanceActive = (guidance.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (guidance.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->velocityQ->navigationActive = (navigation.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XY) || (navigation.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    this->velocityR->guidance = 57.29577951308232088 * guidance.r;
    this->velocityR->navigation = 57.29577951308232088 * navigation.r;
    this->velocityR->guidanceActive = (guidance.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (guidance.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);
    this->velocityR->navigationActive = (navigation.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_Z) || (navigation.conf.velocityPQR.available == IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ);

    auto currentTime = std::chrono::high_resolution_clock::now();
    this->timeNavigation->value = 1e-9 * double(std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - timeOfLatestMessage.navigation).count());
    this->timeGuidance->value = 1e-9 * double(std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - timeOfLatestMessage.guidance).count());
    this->timeGuidanceTrajectory->value = 1e-9 * double(std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - timeOfLatestMessage.guidanceTrajectory).count());
    this->timeGuidanceWaypoint->value = 1e-9 * double(std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - timeOfLatestMessage.guidanceWaypoint).count());
    this->timePolygon->value = 1e-9 * double(std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - timeOfLatestMessage.polygon).count());

    // The actual update for the window
    nanogui::Window::update(ctx);
}

