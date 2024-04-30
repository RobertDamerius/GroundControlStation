#include <VehicleCompass.hpp>
#include <Vehicle.hpp>
#include <AppWindow.hpp>
#include <Configuration.hpp>


#define VEHICLE_COMPASS_CIRCLE_NUM_SLICES        (360)
#define VEHICLE_COMPASS_RULER_START_MINOR_MINOR  (0.93)
#define VEHICLE_COMPASS_RULER_START_MINOR        (0.90)
#define VEHICLE_COMPASS_RULER_START_MAJOR        (0.87)
#define VEHICLE_COMPASS_RULER_END                (0.97)


VehicleCompass::VehicleCompass(){
    this->enable = Configuration::gcs.defaultVehicleStyle.compass.enable;
    this->color.r = static_cast<double>(Configuration::gcs.defaultVehicleStyle.compass.color[0]) / 255.0;
    this->color.g = static_cast<double>(Configuration::gcs.defaultVehicleStyle.compass.color[1]) / 255.0;
    this->color.b = static_cast<double>(Configuration::gcs.defaultVehicleStyle.compass.color[2]) / 255.0;
    this->radius = Configuration::gcs.defaultVehicleStyle.compass.radius;
    this->lineCircle.width = 2.5;
    this->lineRulerMajor.width = 2.5;
    this->lineRulerMinor.width = 2.0;
    this->lineRulerMinorMinor.width = 2.0;
    this->arrowNorth.width = 2.2;
    this->lineHeading.width = 2.5;
    this->headingCommand.width = 3.5;
}

VehicleCompass::~VehicleCompass(){}

void VehicleCompass::GenerateGL(void){
    // Make sure that the GL content is not generated
    DeleteGL();

    // Create and generate circle
    std::vector<LineVertex> vertices;
    LineVertex v;
    v.color[0] = v.color[1] = v.color[2] = 1.0f;
    v.position[1] = 0.0f;
    double angle = 0.0;
    double deltaAngle = 6.283185307179586477 / double(VEHICLE_COMPASS_CIRCLE_NUM_SLICES);
    for(int i = 0; i <= VEHICLE_COMPASS_CIRCLE_NUM_SLICES; i++, angle += deltaAngle){
        v.position[0] = (GLfloat)(this->radius * cos(angle));
        v.position[2] = (GLfloat)(-this->radius * sin(angle));
        vertices.push_back(v);
    }
    lineCircle.SetVertices(vertices);
    lineCircle.Generate();

    // Create and generate ruler
    GLfloat radiusRulerStartMinor = (GLfloat)(VEHICLE_COMPASS_RULER_START_MINOR * this->radius);
    GLfloat radiusRulerStartMajor = (GLfloat)(VEHICLE_COMPASS_RULER_START_MAJOR * this->radius);
    GLfloat radiusRulerStartMinorMinor = (GLfloat)(VEHICLE_COMPASS_RULER_START_MINOR_MINOR * this->radius);
    GLfloat radiusRulerEnd = (GLfloat)(VEHICLE_COMPASS_RULER_END * this->radius);
    vertices.clear();
    v.position[0] = radiusRulerStartMajor;   v.position[2] = 0.0f;                    vertices.push_back(v);
    v.position[0] = radiusRulerEnd;          v.position[2] = 0.0f;                    vertices.push_back(v);
    v.position[0] = 0.0f;                    v.position[2] = radiusRulerStartMajor;   vertices.push_back(v);
    v.position[0] = 0.0f;                    v.position[2] = radiusRulerEnd;          vertices.push_back(v);
    v.position[0] = -radiusRulerStartMajor;  v.position[2] = 0.0f;                    vertices.push_back(v);
    v.position[0] = -radiusRulerEnd;         v.position[2] = 0.0f;                    vertices.push_back(v);
    v.position[0] = 0.0f;                    v.position[2] = -radiusRulerStartMajor;  vertices.push_back(v);
    v.position[0] = 0.0f;                    v.position[2] = -radiusRulerEnd;         vertices.push_back(v);
    lineRulerMajor.SetVertices(vertices);
    lineRulerMajor.Generate();
    vertices.clear();
    v.color[0] = v.color[1] = v.color[2] = 0.8f;
    v.position[0] = 0.866025404f * radiusRulerStartMinor;   v.position[2] = 0.500000000f * radiusRulerStartMinor;   vertices.push_back(v);
    v.position[0] = 0.866025404f * radiusRulerEnd;          v.position[2] = 0.500000000f * radiusRulerEnd;          vertices.push_back(v);
    v.position[0] = 0.500000000f * radiusRulerStartMinor;   v.position[2] = 0.866025404f * radiusRulerStartMinor;   vertices.push_back(v);
    v.position[0] = 0.500000000f * radiusRulerEnd;          v.position[2] = 0.866025404f * radiusRulerEnd;          vertices.push_back(v);
    v.position[0] =-0.500000000f * radiusRulerStartMinor;   v.position[2] = 0.866025404f * radiusRulerStartMinor;   vertices.push_back(v);
    v.position[0] =-0.500000000f * radiusRulerEnd;          v.position[2] = 0.866025404f * radiusRulerEnd;          vertices.push_back(v);
    v.position[0] =-0.866025404f * radiusRulerStartMinor;   v.position[2] = 0.500000000f * radiusRulerStartMinor;   vertices.push_back(v);
    v.position[0] =-0.866025404f * radiusRulerEnd;          v.position[2] = 0.500000000f * radiusRulerEnd;          vertices.push_back(v);
    v.position[0] =-0.866025404f * radiusRulerStartMinor;   v.position[2] =-0.500000000f * radiusRulerStartMinor;   vertices.push_back(v);
    v.position[0] =-0.866025404f * radiusRulerEnd;          v.position[2] =-0.500000000f * radiusRulerEnd;          vertices.push_back(v);
    v.position[0] =-0.500000000f * radiusRulerStartMinor;   v.position[2] =-0.866025404f * radiusRulerStartMinor;   vertices.push_back(v);
    v.position[0] =-0.500000000f * radiusRulerEnd;          v.position[2] =-0.866025404f * radiusRulerEnd;          vertices.push_back(v);
    v.position[0] = 0.500000000f * radiusRulerStartMinor;   v.position[2] =-0.866025404f * radiusRulerStartMinor;   vertices.push_back(v);
    v.position[0] = 0.500000000f * radiusRulerEnd;          v.position[2] =-0.866025404f * radiusRulerEnd;          vertices.push_back(v);
    v.position[0] = 0.866025404f * radiusRulerStartMinor;   v.position[2] =-0.500000000f * radiusRulerStartMinor;   vertices.push_back(v);
    v.position[0] = 0.866025404f * radiusRulerEnd;          v.position[2] =-0.500000000f * radiusRulerEnd;          vertices.push_back(v);
    lineRulerMinor.SetVertices(vertices);
    lineRulerMinor.Generate();
    vertices.clear();
    v.color[0] = v.color[1] = v.color[2] = 0.6f;
    v.position[0] = 0.984807753f * radiusRulerStartMinorMinor;   v.position[2] = 0.173648178f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.984807753f * radiusRulerEnd;               v.position[2] = 0.173648178f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] = 0.939692621f * radiusRulerStartMinorMinor;   v.position[2] = 0.342020143f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.939692621f * radiusRulerEnd;               v.position[2] = 0.342020143f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] = 0.766044443f * radiusRulerStartMinorMinor;   v.position[2] = 0.642787610f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.766044443f * radiusRulerEnd;               v.position[2] = 0.642787610f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] = 0.642787610f * radiusRulerStartMinorMinor;   v.position[2] = 0.766044443f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.642787610f * radiusRulerEnd;               v.position[2] = 0.766044443f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] = 0.342020143f * radiusRulerStartMinorMinor;   v.position[2] = 0.939692621f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.342020143f * radiusRulerEnd;               v.position[2] = 0.939692621f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] = 0.173648178f * radiusRulerStartMinorMinor;   v.position[2] = 0.984807753f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.173648178f * radiusRulerEnd;               v.position[2] = 0.984807753f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.173648178f * radiusRulerStartMinorMinor;   v.position[2] = 0.984807753f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.173648178f * radiusRulerEnd;               v.position[2] = 0.984807753f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.342020143f * radiusRulerStartMinorMinor;   v.position[2] = 0.939692621f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.342020143f * radiusRulerEnd;               v.position[2] = 0.939692621f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.642787610f * radiusRulerStartMinorMinor;   v.position[2] = 0.766044443f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.642787610f * radiusRulerEnd;               v.position[2] = 0.766044443f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.766044443f * radiusRulerStartMinorMinor;   v.position[2] = 0.642787610f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.766044443f * radiusRulerEnd;               v.position[2] = 0.642787610f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.939692621f * radiusRulerStartMinorMinor;   v.position[2] = 0.342020143f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.939692621f * radiusRulerEnd;               v.position[2] = 0.342020143f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.984807753f * radiusRulerStartMinorMinor;   v.position[2] = 0.173648178f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.984807753f * radiusRulerEnd;               v.position[2] = 0.173648178f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] = 0.984807753f * radiusRulerStartMinorMinor;   v.position[2] =-0.173648178f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.984807753f * radiusRulerEnd;               v.position[2] =-0.173648178f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] = 0.939692621f * radiusRulerStartMinorMinor;   v.position[2] =-0.342020143f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.939692621f * radiusRulerEnd;               v.position[2] =-0.342020143f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] = 0.766044443f * radiusRulerStartMinorMinor;   v.position[2] =-0.642787610f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.766044443f * radiusRulerEnd;               v.position[2] =-0.642787610f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] = 0.642787610f * radiusRulerStartMinorMinor;   v.position[2] =-0.766044443f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.642787610f * radiusRulerEnd;               v.position[2] =-0.766044443f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] = 0.342020143f * radiusRulerStartMinorMinor;   v.position[2] =-0.939692621f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.342020143f * radiusRulerEnd;               v.position[2] =-0.939692621f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] = 0.173648178f * radiusRulerStartMinorMinor;   v.position[2] =-0.984807753f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] = 0.173648178f * radiusRulerEnd;               v.position[2] =-0.984807753f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.173648178f * radiusRulerStartMinorMinor;   v.position[2] =-0.984807753f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.173648178f * radiusRulerEnd;               v.position[2] =-0.984807753f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.342020143f * radiusRulerStartMinorMinor;   v.position[2] =-0.939692621f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.342020143f * radiusRulerEnd;               v.position[2] =-0.939692621f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.642787610f * radiusRulerStartMinorMinor;   v.position[2] =-0.766044443f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.642787610f * radiusRulerEnd;               v.position[2] =-0.766044443f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.766044443f * radiusRulerStartMinorMinor;   v.position[2] =-0.642787610f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.766044443f * radiusRulerEnd;               v.position[2] =-0.642787610f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.939692621f * radiusRulerStartMinorMinor;   v.position[2] =-0.342020143f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.939692621f * radiusRulerEnd;               v.position[2] =-0.342020143f * radiusRulerEnd;               vertices.push_back(v);
    v.position[0] =-0.984807753f * radiusRulerStartMinorMinor;   v.position[2] =-0.173648178f * radiusRulerStartMinorMinor;   vertices.push_back(v);
    v.position[0] =-0.984807753f * radiusRulerEnd;               v.position[2] =-0.173648178f * radiusRulerEnd;               vertices.push_back(v);
    lineRulerMinorMinor.SetVertices(vertices);
    lineRulerMinorMinor.Generate();

    // Arrow, north
    GLfloat xOffset = (GLfloat)(1.05 * this->radius);
    GLfloat arrowSize = (GLfloat)(0.05 * this->radius);
    vertices.clear();
    v.color[0] = v.color[1] = v.color[2] = 0.0f;
    v.color[0] = 0.74f;
    v.position[0] = xOffset;                       v.position[2] = 0.0f;        vertices.push_back(v);
    v.position[0] = xOffset;                       v.position[2] = arrowSize;   vertices.push_back(v);
    v.color[0] = 1.0f;
    v.position[0] = xOffset+arrowSize+arrowSize;   v.position[2] = 0.0f;        vertices.push_back(v);
    v.color[0] = 0.74f;
    v.position[0] = xOffset;                       v.position[2] =-arrowSize;   vertices.push_back(v);
    v.position[0] = xOffset;                       v.position[2] = 0.0f;        vertices.push_back(v);
    arrowNorth.SetVertices(vertices);
    arrowNorth.Generate();

    // Line heading
    vertices.clear();
    v.color[0] = v.color[1] = 1.0f; v.color[2] = 0.0f;
    v.position[0] = (GLfloat)(this->radius);  v.position[2] = 0.0f;  vertices.push_back(v);
    v.position[0] = 0.0f;                     v.position[2] = 0.0f;  vertices.push_back(v);
    lineHeading.SetVertices(vertices);
    lineHeading.Generate();

    // Heading command
    xOffset = (GLfloat)this->radius;
    GLfloat symbolSizeX = (GLfloat)(0.05 * this->radius);
    GLfloat symbolSizeY = (GLfloat)(0.03 * this->radius);
    vertices.clear();
    v.color[0] = v.color[2] = 1.0f; v.color[1] = 0.0f;
    v.position[0] = xOffset-symbolSizeX;   v.position[2] = 0.0f;         vertices.push_back(v);
    v.position[0] = xOffset;               v.position[2] = symbolSizeY;  vertices.push_back(v);
    v.position[0] = xOffset+symbolSizeX;   v.position[2] = 0.0f;         vertices.push_back(v);
    v.position[0] = xOffset;               v.position[2] =-symbolSizeY;  vertices.push_back(v);
    v.position[0] = xOffset-symbolSizeX;   v.position[2] = 0.0f;         vertices.push_back(v);
    v.position[0] = xOffset;               v.position[2] = symbolSizeY;  vertices.push_back(v);
    headingCommand.SetVertices(vertices);
    headingCommand.Generate();
}

void VehicleCompass::DeleteGL(void){
    lineCircle.Delete();
    lineRulerMinorMinor.Delete();
    lineRulerMinor.Delete();
    lineRulerMajor.Delete();
    arrowNorth.Delete();
    lineHeading.Delete();
    headingCommand.Delete();
}

void VehicleCompass::DrawLines(ShaderLine& shaderLine, ShaderLineStrip& shaderLineStrip, Vehicle& parent){
    if(!enable)
        return;

    // Translation matrix
    glm::mat4 matTranslation(1.0f);
    matTranslation = glm::translate(matTranslation, glm::vec3(parent.renderingState.position.x, -parent.renderingState.position.z, parent.renderingState.position.y));
    glm::mat4 modelMatHeadingLine = glm::rotate(matTranslation, (GLfloat)parent.renderingState.yaw(), glm::vec3(0.0f,-1.0f,0.0f));
    glm::mat4 modelMatHeadingCommand = glm::rotate(matTranslation, (GLfloat)parent.guidance.yaw(), glm::vec3(0.0f,-1.0f,0.0f));

    // Draw ruler
    shaderLine.Use();
    shaderLine.SetColor(color);
    shaderLine.SetModelMatrix(matTranslation);
    shaderLine.SetLineWidth(lineRulerMinorMinor.width);
    lineRulerMinorMinor.Draw();
    shaderLine.SetLineWidth(lineRulerMinor.width);
    lineRulerMinor.Draw();
    shaderLine.SetLineWidth(lineRulerMajor.width);
    lineRulerMajor.Draw();

    // Draw circle
    shaderLineStrip.Use();
    shaderLineStrip.SetColor(color);
    shaderLineStrip.SetModelMatrix(matTranslation);
    shaderLineStrip.SetLineWidth(lineCircle.width);
    shaderLineStrip.SetPositionOffset(glm::vec3(0.0f));
    lineCircle.Draw();

    // Draw arrow north
    shaderLineStrip.SetLineWidth(arrowNorth.width);
    shaderLineStrip.SetColor(glm::vec3(1.0f));
    arrowNorth.Draw();

    // Draw heading line
    shaderLine.Use();
    shaderLine.SetColor(glm::vec3(1.0f));
    shaderLine.SetModelMatrix(modelMatHeadingLine);
    shaderLine.SetLineWidth(lineHeading.width);
    lineHeading.Draw();

    // Draw heading command
    shaderLineStrip.Use();
    shaderLineStrip.SetModelMatrix(modelMatHeadingCommand);
    shaderLineStrip.SetLineWidth(headingCommand.width);
    headingCommand.Draw();
}

double VehicleCompass::Resize(double r){
    this->radius = (r > 0.001) ? r : 0.001;
    GenerateGL();
    return this->radius;
}

double VehicleCompass::SetRadius(double r){
    this->radius = (r > 0.001) ? r : 0.001;
    return this->radius;
}

double VehicleCompass::GetRadius(void){
    return this->radius;
}

VehicleCompass& VehicleCompass::operator=(const VehicleCompass& rhs){
    this->enable = rhs.enable;
    this->color = rhs.color;
    this->lineCircle = rhs.lineCircle;
    this->lineRulerMajor = rhs.lineRulerMajor;
    this->lineRulerMinor = rhs.lineRulerMinor;
    this->lineRulerMinorMinor = rhs.lineRulerMinorMinor;
    this->arrowNorth = rhs.arrowNorth;
    this->lineHeading = rhs.lineHeading;
    this->headingCommand = rhs.headingCommand;
    return *this;
}

