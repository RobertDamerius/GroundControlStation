#include <IMPConvexPolygon.hpp>


IMP::ConvexPolygon::ConvexPolygon(){
    polygonID = 0;
    classification = 0x00;
    velocityNorth = 0.0;
    velocityEast = 0.0;
    yawRate = 0.0;
}

IMP::ConvexPolygon::~ConvexPolygon(){}

int IMP::ConvexPolygon::Encode(uint8_t* bytes, const uint32_t length){
    // Error if invalid buffer or input buffer is too small
    if(!bytes || (length < 3)){
        return -1;
    }
    int index = 0;

    // Lambda functions for data type encoding
    auto lambdaEncodeFloat = [&bytes, &index](double value){
        union {
            float f;
            uint8_t b[4];
            uint32_t u;
        } un;
        un.f = (float)value;
        un.u = IMPSwapByteOrder(un.u);
        memcpy(&bytes[index], &un.b[0], 4);
        index += 4;
    };

    // Polygon ID
    bytes[0] = (uint8_t)(0x00FF & (polygonID >> 8));
    bytes[1] = (uint8_t)(0x00FF & polygonID);

    // Classification
    bytes[2] = classification;
    index = 3;

    // Velocities (only for dynamic polygons)
    if(0x80 & classification){
        if(uint32_t(index + 12) > length){
            return -1;
        }
        lambdaEncodeFloat(velocityNorth);
        lambdaEncodeFloat(velocityEast);
        lambdaEncodeFloat(yawRate);
    }

    // Number of vertices
    if(vertices.size() > 255){
        vertices.resize(255);
    }
    uint32_t numVertices = 0x000000FF & ((uint32_t)vertices.size());
    if(uint32_t(index + 1 + 8*(int)numVertices) > length){
        return -1;
    }
    bytes[index++] = (uint8_t)(numVertices);

    // Vertex data
    for(uint32_t v = 0; v < numVertices; v++){
        lambdaEncodeFloat(vertices[v][0]);
        lambdaEncodeFloat(vertices[v][1]);
    }

    // Current index indicates length
    return index;
}

int IMP::ConvexPolygon::Decode(const uint8_t* bytes, const uint32_t length){
    // Reset state attributes
    polygonID = 0;
    classification = 0x00;
    velocityNorth = 0.0;
    velocityEast = 0.0;
    yawRate = 0.0;
    vertices.clear();

    // Error if invalid buffer or input buffer is too small
    if(!bytes || (length < 3)){
        return -1;
    }
    int index = 0;

    // Lambda functions for data type decoding
    auto lambdaDecodeFloat = [&bytes, &index](){
        union {
            float f;
            uint8_t b[4];
            uint32_t u;
        } un;
        memcpy(&un.b[0], &bytes[index], 4);
        un.u = IMPSwapByteOrder(un.u);
        index += 4;
        return (double)un.f;
    };

    // Polygon ID
    polygonID = (uint16_t(bytes[0]) << 8) | (uint16_t)bytes[1];

    // Classification
    classification = bytes[2];
    index = 3;

    // Velocities (only for dynamic polygons)
    if(0x80 & classification){
        if(uint32_t(index + 12) > length){
            return -1;
        }
        velocityNorth = lambdaDecodeFloat();
        velocityEast = lambdaDecodeFloat();
        yawRate = lambdaDecodeFloat();
    }

    // Number of vertices
    if(uint32_t(index + 1) > length){
        return -1;
    }
    uint32_t numVertices = (uint32_t)bytes[index++];
    if(uint32_t(index + 8*(int)numVertices) > length){
        return -1;
    }

    // Vertex data
    vertices.resize(numVertices);
    for(uint32_t v = 0; v < numVertices; v++){
        vertices[v][0] = lambdaDecodeFloat();
        vertices[v][1] = lambdaDecodeFloat();
    }

    // Current index indicates length
    return index;
}

