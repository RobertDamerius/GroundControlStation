#include <GroundPlane.hpp>
#include <AppWindow.hpp>
#include <Configuration.hpp>


GroundPlane::GroundPlane(){
    Reset();
}

GroundPlane::~GroundPlane(){}

void GroundPlane::Draw(void){
    AppWindow::DrawScreenQuad();
}

void GroundPlane::Reset(void){
    color.r = 134.0 / 255.0;
    color.g = 169.0 / 255.0;
    color.b = 182.0 / 255.0;
}

