#include <PolyMesh.hpp>
#ifndef DEBUG_GLCHECK
#define DEBUG_GLCHECK(x) x
#endif


static std::vector<std::string> SplitLine(std::string& line){
    std::vector<std::string> result;
    std::string strValue;
    std::istringstream lineStream(line);
    while(std::getline(lineStream, strValue, ' ')){
        result.push_back(strValue);
    }
    return result;
}

PolyMesh::PolyMesh(){
    vao = 0;
    vbo = 0;
    ebo = 0;
}

bool PolyMesh::Read(std::string filename, std::string& err){
    // Open file and read lines
    this->indices.clear();
    std::ifstream file(filename);
    file.unsetf(std::ios::skipws);
    if(!file.is_open()){
        err = std::string("Could not open \"") + filename + std::string("\"!");
        return false;
    }
    std::string line;
    std::vector<std::string> lines;
    while(std::getline(file, line)){
        auto istart = line.find_first_not_of(" \n\r\t\f\v");
        line = (istart == std::string::npos) ? "" : line.substr(istart);
        auto iend = line.find_last_not_of(" \n\r\t\f\v");
        line = (iend == std::string::npos) ? "" : line.substr(0, iend + 1);
        if(!line.length()) continue;
        if(!line.substr(0,7).compare("comment")) continue;
        lines.push_back(line);
    }
    file.close();

    // Decode header
    if((lines.size() < 3) || lines[0].compare("ply") || lines[1].compare("format ascii 1.0")){
        err = std::string("Invalid format of ply file \"") + filename + std::string("\"!");
        return false;
    }
    auto elementVertex = SplitLine(lines[2]);
    if((3 != elementVertex.size()) || elementVertex[0].compare("element") || elementVertex[1].compare("vertex")){
        err = std::string("Invalid keyword in ply file \"") + filename + std::string("\"!");
        return false;
    }
    uint32_t numVertices = 0;
    try{ numVertices = std::stoi(elementVertex[2]); }
    catch(...){
        err = std::string("Invalid format of ply file \"") + filename + std::string("\"!");
        return false;
    }
    int index = 3;
    int numVertexProperties = 0;
    int ix = -1, iy = -1, iz = -1, inx = -1, iny = -1, inz = -1, ir = -1, ig = -1, ib = -1;
    for(int i = 0; index < (int)lines.size(); index++, i++){
        if(lines[index].substr(0,8).compare("property")){
            break;
        }
        auto property = SplitLine(lines[index]);
        if(3 != property.size()){
            err = std::string("Unsuported vertex property format of ply file \"") + filename + std::string("\"!");
            return false;
        }
        if(!property[2].compare("x")){
            if(property[1].compare("float")){
                err = std::string("Unsuported data type for vertex property x of ply file \"") + filename + std::string("\"!");
                return false;
            }
            ix = i;
        }
        else if(!property[2].compare("y")){
            if(property[1].compare("float")){
                err = std::string("Unsuported data type for vertex property y of ply file \"") + filename + std::string("\"!");
                return false;
            }
            iy = i;
        }
        else if(!property[2].compare("z")){
            if(property[1].compare("float")){
                err = std::string("Unsuported data type for vertex property z of ply file \"") + filename + std::string("\"!");
                return false;
            }
            iz = i;
        }
        else if(!property[2].compare("nx")){
            if(property[1].compare("float")){
                err = std::string("Unsuported data type for vertex property nx of ply file \"") + filename + std::string("\"!");
                return false;
            }
            inx = i;
        }
        else if(!property[2].compare("ny")){
            if(property[1].compare("float")){
                err = std::string("Unsuported data type for vertex property ny of ply file \"") + filename + std::string("\"!");
                return false;
            }
            iny = i;
        }
        else if(!property[2].compare("nz")){
            if(property[1].compare("float")){
                err = std::string("Unsuported data type for vertex property nz of ply file \"") + filename + std::string("\"!");
                return false;
            }
            inz = i;
        }
        else if(!property[2].compare("red") || !property[2].compare("r")){
            if(property[1].compare("uchar")){
                err = std::string("Unsuported data type for vertex property red of ply file \"") + filename + std::string("\"!");
                return false;
            }
            ir = i;
        }
        else if(!property[2].compare("green") || !property[2].compare("g")){
            if(property[1].compare("uchar")){
                err = std::string("Unsuported data type for vertex property green of ply file \"") + filename + std::string("\"!");
                return false;
            }
            ig = i;
        }
        else if(!property[2].compare("blue") || !property[2].compare("b")){
            if(property[1].compare("uchar")){
                err = std::string("Unsuported data type for vertex property blue of ply file \"") + filename + std::string("\"!");
                return false;
            }
            ib = i;
        }
        numVertexProperties++;
    }
    if((index + 2) >= (int)lines.size()){
        err = std::string("Invalid format of ply file \"") + filename + std::string("\"!");
        return false;
    }
    auto elementFace = SplitLine(lines[index]);
    if((3 != elementFace.size()) || elementFace[0].compare("element") || elementFace[1].compare("face")){
        err = std::string("Invalid keyword in ply file \"") + filename + std::string("\"!");
        return false;
    }
    uint32_t numFaces = 0;
    try{ numFaces = std::stoi(elementFace[2]); }
    catch(...){
        err = std::string("Invalid format of ply file \"") + filename + std::string("\"!");
        return false;
    }
    if(lines[index + 1].compare("property list uchar uint vertex_indices") || lines[index + 2].compare("end_header")){
        err = std::string("Unsuported property format for face elements of ply file \"") + filename + std::string("\"!");
        return false;
    }
    index += 3;

    // Decode data
    std::vector<PolyMeshVertex> listVertices;
    if((uint32_t(index) + numVertices + numFaces) != (uint32_t)lines.size()){
        err = std::string("Invalid format of ply file \"") + filename + std::string("\"!");
        return false;
    }
    for(uint32_t n = 0; n < numVertices; n++, index++){
        auto values = SplitLine(lines[index]);
        if(numVertexProperties != (int)values.size()){
            err = std::string("Invalid format of ply file \"") + filename + std::string("\"!");
            return false;
        }
        listVertices.push_back(PolyMeshVertex());
        listVertices.back().position[0] = 0.0f;
        listVertices.back().position[1] = 0.0f;
        listVertices.back().position[2] = 0.0f;
        listVertices.back().normal[0] = 0.0f;
        listVertices.back().normal[1] = 1.0f;
        listVertices.back().normal[2] = 0.0f;
        listVertices.back().color[0] = 0.678f;
        listVertices.back().color[1] = 0.678f;
        listVertices.back().color[2] = 0.678f;
        if((ix >= 0) && (ix < numVertexProperties)){ listVertices.back().position[0] = std::stof(values[ix]); }
        if((iy >= 0) && (iy < numVertexProperties)){ listVertices.back().position[1] = std::stof(values[iy]); }
        if((iz >= 0) && (iz < numVertexProperties)){ listVertices.back().position[2] = std::stof(values[iz]); }
        if((inx >= 0) && (inx < numVertexProperties)){ listVertices.back().normal[0] = std::stof(values[inx]); }
        if((iny >= 0) && (iny < numVertexProperties)){ listVertices.back().normal[1] = std::stof(values[iny]); }
        if((inz >= 0) && (inz < numVertexProperties)){ listVertices.back().normal[2] = std::stof(values[inz]); }
        if((ir >= 0) && (ir < numVertexProperties)){ listVertices.back().color[0] = std::stof(values[ir]) / 255.0f; }
        if((ig >= 0) && (ig < numVertexProperties)){ listVertices.back().color[1] = std::stof(values[ig]) / 255.0f; }
        if((ib >= 0) && (ib < numVertexProperties)){ listVertices.back().color[2] = std::stof(values[ib]) / 255.0f; }
    }
    std::vector<GLuint> listIndices;
    for(uint32_t n = 0; n < numFaces; n++, index++){
        auto values = SplitLine(lines[index]);
        if((4 != values.size()) || values[0].compare("3")){
            err = std::string("Faces of ply file \"") + filename + std::string("\" are not triangulated!");
            return false;
        }
        try{
            listIndices.push_back((GLuint)(std::stoul(values[1]))); if(listIndices.back() >= listVertices.size()) throw(0);
            listIndices.push_back((GLuint)(std::stoul(values[2]))); if(listIndices.back() >= listVertices.size()) throw(0);
            listIndices.push_back((GLuint)(std::stoul(values[3]))); if(listIndices.back() >= listVertices.size()) throw(0);
        }
        catch(...){
            err = std::string("Invalid format of ply file \"") + filename + std::string("\"!");
            return false;
        }
    }
    this->indices.swap(listIndices);
    this->vertices.swap(listVertices);
    return true;
}

void PolyMesh::GenerateGL(void){
    DeleteGL();
    DEBUG_GLCHECK( glGenVertexArrays(1, &vao); );
    DEBUG_GLCHECK( glGenBuffers(1, &vbo); );
    DEBUG_GLCHECK( glGenBuffers(1, &ebo); );
    DEBUG_GLCHECK( glBindVertexArray(vao); );
        DEBUG_GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vbo); );
        DEBUG_GLCHECK( glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PolyMeshVertex), &(vertices[0]), GL_STATIC_DRAW); );
        DEBUG_GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); );
        DEBUG_GLCHECK( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &(indices[0]), GL_STATIC_DRAW); );
        // Position attribute
        DEBUG_GLCHECK( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PolyMeshVertex), (GLvoid*)0); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(0); );
        // Normals
        DEBUG_GLCHECK( glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PolyMeshVertex), (GLvoid*)offsetof(PolyMeshVertex, normal)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(1); );
        // Color
        DEBUG_GLCHECK( glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PolyMeshVertex), (GLvoid*)offsetof(PolyMeshVertex, color)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(2); );
    DEBUG_GLCHECK( glBindVertexArray(0); );
}

void PolyMesh::DeleteGL(void){
    if(vao) glDeleteVertexArrays(1, &vao);
    if(vbo) glDeleteBuffers(1, &vbo);
    if(ebo) glDeleteBuffers(1, &ebo);
    vao = 0;
    vbo = 0;
    ebo = 0;
}

void PolyMesh::DrawTriangles(void){
    DEBUG_GLCHECK( glBindVertexArray(vao); );
    DEBUG_GLCHECK( glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLvoid*)0); );
}

