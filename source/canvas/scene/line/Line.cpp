#include <Line.hpp>


Line::Line(): LinePrimitive(LINE_PRIMITIVE_TYPE_LINES){}

Line::~Line(){}

void Line::SetVertices(std::vector<LineVertex>& v){
    if(v.size() % 2){
        v.pop_back();
    }
    this->vertices.swap(v);
    this->vertices.shrink_to_fit();
}

Line& Line::operator=(const Line& rhs){
    LinePrimitive::operator=(rhs);
    return *this;
}

