#include <PositionHistory.hpp>
#include <Configuration.hpp>


PositionHistory::PositionHistory(){
    this->width = Configuration::gcs.defaultVehicleStyle.positionHistory.lineWidth;
    this->enable = Configuration::gcs.defaultVehicleStyle.positionHistory.enable;
    this->maximumSOG = Configuration::gcs.defaultVehicleStyle.positionHistory.maximumSOG;
    this->timePeriodMs = Configuration::gcs.defaultVehicleStyle.positionHistory.updatePeriodMs;
    this->timePeriod = 0.001 * double(this->timePeriodMs);
    this->t = 0.0;
    this->defaultBufferSize = Configuration::gcs.defaultVehicleStyle.positionHistory.bufferSize;
    generated = false;
}

PositionHistory::~PositionHistory(){}

void PositionHistory::Generate(glm::vec3 initialPosition){
    CircularLineBuffer::Generate(defaultBufferSize, initialPosition);
    generated = true;
}

void PositionHistory::Delete(void){
    CircularLineBuffer::Delete();
    generated = false;
}

PositionHistory& PositionHistory::operator=(const PositionHistory& rhs){
    CircularLineBuffer::operator=(rhs);
    this->enable = rhs.enable;
    this->maximumSOG = rhs.maximumSOG;
    this->timePeriodMs = rhs.timePeriodMs;
    this->timePeriod = rhs.timePeriod;
    this->t = rhs.t;
    return *this;
}

void PositionHistory::Update(double dt, glm::vec3 position, double sog){
    this->t += dt;
    while(this->t >= this->timePeriod){
        this->t -= this->timePeriod;
        CircularLineBuffer::Add(glm::vec3(position.x, -position.z, position.y), (GLfloat)sog);
    }
}

void PositionHistory::Draw(ShaderCircularLineBuffer& shader){
    if(enable){
        shader.SetLineWidth(this->width);
        shader.SetValueScale((GLfloat)(1.0 / this->maximumSOG));
        CircularLineBuffer::Draw();
    }
}

void PositionHistory::SetMaximumSOG(double value){
    this->maximumSOG = (value > 0.001) ? value : 0.001;
}

void PositionHistory::SetTimePeriodMs(uint32_t t){
    this->timePeriod = 0.001 * (double)(this->timePeriodMs = t);
}

