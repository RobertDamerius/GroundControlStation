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
    color = Configuration::style.displayGroundColor;
}

