#include <AxisAlignedBoundingBox.hpp>


AxisAlignedBoundingBox::AxisAlignedBoundingBox(glm::dvec3 p, glm::dvec3 d):position(p), dimension(d){}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox(){}

AxisAlignedBoundingBox AxisAlignedBoundingBox::operator+(const AxisAlignedBoundingBox& rhs){
    AxisAlignedBoundingBox result = *this;
    result += rhs;
    return result;
}

AxisAlignedBoundingBox AxisAlignedBoundingBox::operator+=(const AxisAlignedBoundingBox& rhs){
    glm::dvec3 maxPosThis = position + dimension;
    glm::dvec3 maxPosition = rhs.position + rhs.dimension;
    maxPosition.x = (maxPosThis.x > maxPosition.x) ? maxPosThis.x : maxPosition.x;
    maxPosition.y = (maxPosThis.y > maxPosition.y) ? maxPosThis.y : maxPosition.y;
    maxPosition.z = (maxPosThis.z > maxPosition.z) ? maxPosThis.z : maxPosition.z;
    position.x = (position.x < rhs.position.x) ? position.x : rhs.position.x;
    position.y = (position.y < rhs.position.y) ? position.y : rhs.position.y;
    position.z = (position.z < rhs.position.z) ? position.z : rhs.position.z;
    dimension = maxPosition - position;
    return *this;
}

AxisAlignedBoundingBox operator*(glm::mat4& lhs, AxisAlignedBoundingBox& rhs){
    AxisAlignedBoundingBox result;
    glm::dvec4 posMin = glm::dmat4(lhs) * glm::dvec4(rhs.position, 1.0);
    glm::dvec4 p1 = glm::dmat4(lhs) * glm::dvec4(rhs.position + glm::dvec3(0.0, 0.0, rhs.dimension.z), 1.0);
    glm::dvec4 p2 = glm::dmat4(lhs) * glm::dvec4(rhs.position + glm::dvec3(rhs.dimension.x, 0.0, rhs.dimension.z), 1.0);
    glm::dvec4 p3 = glm::dmat4(lhs) * glm::dvec4(rhs.position + glm::dvec3(rhs.dimension.x, 0.0, 0.0), 1.0);
    glm::dvec4 p4 = glm::dmat4(lhs) * glm::dvec4(rhs.position + glm::dvec3(0.0, rhs.dimension.y, 0.0), 1.0);
    glm::dvec4 p5 = glm::dmat4(lhs) * glm::dvec4(rhs.position + glm::dvec3(0.0, rhs.dimension.y, rhs.dimension.z), 1.0);
    glm::dvec4 p6 = glm::dmat4(lhs) * glm::dvec4(rhs.position + glm::dvec3(rhs.dimension.x, rhs.dimension.y, rhs.dimension.z), 1.0);
    glm::dvec4 p7 = glm::dmat4(lhs) * glm::dvec4(rhs.position + glm::dvec3(rhs.dimension.x, rhs.dimension.y, 0.0), 1.0);
    glm::dvec4 posMax = posMin;

    posMin.x = (p1.x < posMin.x) ? p1.x : posMin.x;
    posMin.y = (p1.y < posMin.y) ? p1.y : posMin.y;
    posMin.z = (p1.z < posMin.z) ? p1.z : posMin.z;
    posMin.x = (p2.x < posMin.x) ? p2.x : posMin.x;
    posMin.y = (p2.y < posMin.y) ? p2.y : posMin.y;
    posMin.z = (p2.z < posMin.z) ? p2.z : posMin.z;
    posMin.x = (p3.x < posMin.x) ? p3.x : posMin.x;
    posMin.y = (p3.y < posMin.y) ? p3.y : posMin.y;
    posMin.z = (p3.z < posMin.z) ? p3.z : posMin.z;
    posMin.x = (p4.x < posMin.x) ? p4.x : posMin.x;
    posMin.y = (p4.y < posMin.y) ? p4.y : posMin.y;
    posMin.z = (p4.z < posMin.z) ? p4.z : posMin.z;
    posMin.x = (p5.x < posMin.x) ? p5.x : posMin.x;
    posMin.y = (p5.y < posMin.y) ? p5.y : posMin.y;
    posMin.z = (p5.z < posMin.z) ? p5.z : posMin.z;
    posMin.x = (p6.x < posMin.x) ? p6.x : posMin.x;
    posMin.y = (p6.y < posMin.y) ? p6.y : posMin.y;
    posMin.z = (p6.z < posMin.z) ? p6.z : posMin.z;
    posMin.x = (p7.x < posMin.x) ? p7.x : posMin.x;
    posMin.y = (p7.y < posMin.y) ? p7.y : posMin.y;
    posMin.z = (p7.z < posMin.z) ? p7.z : posMin.z;

    posMax.x = (p1.x > posMax.x) ? p1.x : posMax.x;
    posMax.y = (p1.y > posMax.y) ? p1.y : posMax.y;
    posMax.z = (p1.z > posMax.z) ? p1.z : posMax.z;
    posMax.x = (p2.x > posMax.x) ? p2.x : posMax.x;
    posMax.y = (p2.y > posMax.y) ? p2.y : posMax.y;
    posMax.z = (p2.z > posMax.z) ? p2.z : posMax.z;
    posMax.x = (p3.x > posMax.x) ? p3.x : posMax.x;
    posMax.y = (p3.y > posMax.y) ? p3.y : posMax.y;
    posMax.z = (p3.z > posMax.z) ? p3.z : posMax.z;
    posMax.x = (p4.x > posMax.x) ? p4.x : posMax.x;
    posMax.y = (p4.y > posMax.y) ? p4.y : posMax.y;
    posMax.z = (p4.z > posMax.z) ? p4.z : posMax.z;
    posMax.x = (p5.x > posMax.x) ? p5.x : posMax.x;
    posMax.y = (p5.y > posMax.y) ? p5.y : posMax.y;
    posMax.z = (p5.z > posMax.z) ? p5.z : posMax.z;
    posMax.x = (p6.x > posMax.x) ? p6.x : posMax.x;
    posMax.y = (p6.y > posMax.y) ? p6.y : posMax.y;
    posMax.z = (p6.z > posMax.z) ? p6.z : posMax.z;
    posMax.x = (p7.x > posMax.x) ? p7.x : posMax.x;
    posMax.y = (p7.y > posMax.y) ? p7.y : posMax.y;
    posMax.z = (p7.z > posMax.z) ? p7.z : posMax.z;

    result.position = posMin;
    result.dimension = posMax - posMin;
    return result;
}

