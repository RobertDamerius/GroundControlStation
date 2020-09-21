#include <Angle.hpp>


Angle::Angle(double value): value(SymmetricalRange(value)){}

Angle::~Angle(){}

double Angle::Deg(void){
    return 57.29577951308232088 * this->value;
}

double Angle::PositiveDeg(void){
    return 57.29577951308232088 * ((this->value < 0.0) ? (this->value + 6.28318530717959) : this->value);
}

double Angle::SymmetricalRange(double d){
    int i = (int)(d * 0.159154943091895);
    d -= 6.28318530717959 * (double)i;
    return (d < -3.14159265358979) ? (d + 6.28318530717959) : ((d >= 3.14159265358979) ? (d - 6.28318530717959) : d);
}

Angle& Angle::operator=(const Angle& rhs){
    this->value = rhs.value;
    return *this;
}

Angle& Angle::operator=(const double& rhs){
    this->value = SymmetricalRange(rhs);
    return *this;
}

double Angle::operator()(void){
    return this->value;
}

Angle Angle::operator+(const Angle& rhs){
    Angle result;
    result.value = SymmetricalRange(this->value + rhs.value);
    return result;
}

Angle Angle::operator+(const double& rhs){
    Angle result;
    result.value = SymmetricalRange(this->value + rhs);
    return result;
}

Angle Angle::operator-(const Angle& rhs){
    Angle result;
    result.value = SymmetricalRange(this->value - rhs.value);
    return result;
}

Angle Angle::operator-(const double& rhs){
    Angle result;
    result.value = SymmetricalRange(this->value - rhs);
    return result;
}

Angle Angle::operator-(void){
    Angle result;
    result.value = SymmetricalRange(-this->value);
    return result;
}

Angle Angle::operator*(const double& rhs){
    Angle result;
    result.value = SymmetricalRange(this->value * rhs);
    return result;
}

Angle Angle::operator/(const double& rhs){
    Angle result;
    result.value = SymmetricalRange(this->value / rhs);
    return result;
}

Angle& Angle::operator+=(const Angle& rhs){
    this->value = SymmetricalRange(this->value + rhs.value);
    return *this;
}

Angle& Angle::operator+=(const double& rhs){
    this->value = SymmetricalRange(this->value + rhs);
    return *this;
}

Angle& Angle::operator-=(const Angle& rhs){
    this->value = SymmetricalRange(this->value - rhs.value);
    return *this;
}

Angle& Angle::operator-=(const double& rhs){
    this->value = SymmetricalRange(this->value - rhs);
    return *this;
}

Angle& Angle::operator*=(const Angle& rhs){
    this->value = SymmetricalRange(this->value * rhs.value);
    return *this;
}

Angle& Angle::operator*=(const double& rhs){
    this->value = SymmetricalRange(this->value * rhs);
    return *this;
}

Angle& Angle::operator/=(const Angle& rhs){
    this->value = SymmetricalRange(this->value / rhs.value);
    return *this;
}

Angle& Angle::operator/=(const double& rhs){
    this->value = SymmetricalRange(this->value / rhs);
    return *this;
}

Angle operator+(double lvalue, const Angle& rvalue){
    Angle result;
    result.value = Angle::SymmetricalRange(lvalue + rvalue.value);
    return result;
}

Angle operator-(double lvalue, const Angle& rvalue){
    Angle result;
    result.value = Angle::SymmetricalRange(lvalue - rvalue.value);
    return result;
}

Angle operator*(double lvalue, const Angle& rvalue){
    Angle result;
    result.value = Angle::SymmetricalRange(lvalue * rvalue.value);
    return result;
}

Angle operator/(double lvalue, const Angle& rvalue){
    Angle result;
    result.value = Angle::SymmetricalRange(lvalue / rvalue.value);
    return result;
}

