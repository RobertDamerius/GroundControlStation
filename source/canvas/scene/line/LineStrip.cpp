#include <LineStrip.hpp>


LineStrip::LineStrip(): LinePrimitive(LINE_PRIMITIVE_TYPE_LINE_STRIP_ADJACENCY){}

LineStrip::~LineStrip(){}

void LineStrip::SetVertices(std::vector<LineVertex>& v){
    this->vertices.swap(v);
    if(this->vertices.size() > 1){
        // Positions of first two vertices
        glm::dvec3 p0(double(this->vertices[0].position[0]), double(this->vertices[0].position[1]), double(this->vertices[0].position[2]));
        glm::dvec3 p1(double(this->vertices[1].position[0]), double(this->vertices[1].position[1]), double(this->vertices[1].position[2]));

        // Extrapolate and insert new vertex at beginning
        glm::dvec3 n = p0 - p1;
        double d = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
        n *= 1.0 / ((d < 1e-6) ? 1e-6 : d);
        glm::dvec3 p = p0 + n * 1e-4;
        this->vertices.insert(this->vertices.begin(), LineVertex());
        this->vertices[0].position[0] = (GLfloat)p.x;
        this->vertices[0].position[1] = (GLfloat)p.y;
        this->vertices[0].position[2] = (GLfloat)p.z;
        this->vertices[0].color[0] = this->vertices[1].color[0];
        this->vertices[0].color[1] = this->vertices[1].color[1];
        this->vertices[0].color[2] = this->vertices[1].color[2];

        // Positions of last two vertices
        p0 = glm::dvec3(this->vertices[this->vertices.size()-2].position[0], this->vertices[this->vertices.size()-2].position[1], this->vertices[this->vertices.size()-2].position[2]);
        p1 = glm::dvec3(this->vertices[this->vertices.size()-1].position[0], this->vertices[this->vertices.size()-1].position[1], this->vertices[this->vertices.size()-1].position[2]);

        // Extrapolate and insert new vertex at end
        n = p1 - p0;
        d = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
        n *= 1.0 / ((d < 1e-6) ? 1e-6 : d);
        p = p1 + n * 1e-4;
        this->vertices.push_back(LineVertex());
        this->vertices.back().position[0] = (GLfloat)p.x;
        this->vertices.back().position[1] = (GLfloat)p.y;
        this->vertices.back().position[2] = (GLfloat)p.z;
        this->vertices.back().color[0] = this->vertices[this->vertices.size()-2].color[0];
        this->vertices.back().color[1] = this->vertices[this->vertices.size()-2].color[1];
        this->vertices.back().color[2] = this->vertices[this->vertices.size()-2].color[2];
    }
    else{
        this->vertices.clear();
    }
    this->vertices.shrink_to_fit();
}

LineStrip& LineStrip::operator=(const LineStrip& rhs){
    LinePrimitive::operator=(rhs);
    return *this;
}

