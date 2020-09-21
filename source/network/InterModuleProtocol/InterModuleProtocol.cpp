#include <InterModuleProtocol.hpp>


#define CLAMP(x, lo, hi) ((x < lo) ? lo : ((x > hi) ? hi : x))


static const uint16_t __lut_crc16[] = {
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,0x8108,0x9129,0xA14A,0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF,
    0x1231,0x0210,0x3273,0x2252,0x52B5,0x4294,0x72F7,0x62D6,0x9339,0x8318,0xB37B,0xA35A,0xD3BD,0xC39C,0xF3FF,0xE3DE,
    0x2462,0x3443,0x0420,0x1401,0x64E6,0x74C7,0x44A4,0x5485,0xA56A,0xB54B,0x8528,0x9509,0xE5EE,0xF5CF,0xC5AC,0xD58D,
    0x3653,0x2672,0x1611,0x0630,0x76D7,0x66F6,0x5695,0x46B4,0xB75B,0xA77A,0x9719,0x8738,0xF7DF,0xE7FE,0xD79D,0xC7BC,
    0x48C4,0x58E5,0x6886,0x78A7,0x0840,0x1861,0x2802,0x3823,0xC9CC,0xD9ED,0xE98E,0xF9AF,0x8948,0x9969,0xA90A,0xB92B,
    0x5AF5,0x4AD4,0x7AB7,0x6A96,0x1A71,0x0A50,0x3A33,0x2A12,0xDBFD,0xCBDC,0xFBBF,0xEB9E,0x9B79,0x8B58,0xBB3B,0xAB1A,
    0x6CA6,0x7C87,0x4CE4,0x5CC5,0x2C22,0x3C03,0x0C60,0x1C41,0xEDAE,0xFD8F,0xCDEC,0xDDCD,0xAD2A,0xBD0B,0x8D68,0x9D49,
    0x7E97,0x6EB6,0x5ED5,0x4EF4,0x3E13,0x2E32,0x1E51,0x0E70,0xFF9F,0xEFBE,0xDFDD,0xCFFC,0xBF1B,0xAF3A,0x9F59,0x8F78,
    0x9188,0x81A9,0xB1CA,0xA1EB,0xD10C,0xC12D,0xF14E,0xE16F,0x1080,0x00A1,0x30C2,0x20E3,0x5004,0x4025,0x7046,0x6067,
    0x83B9,0x9398,0xA3FB,0xB3DA,0xC33D,0xD31C,0xE37F,0xF35E,0x02B1,0x1290,0x22F3,0x32D2,0x4235,0x5214,0x6277,0x7256,
    0xB5EA,0xA5CB,0x95A8,0x8589,0xF56E,0xE54F,0xD52C,0xC50D,0x34E2,0x24C3,0x14A0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xA7DB,0xB7FA,0x8799,0x97B8,0xE75F,0xF77E,0xC71D,0xD73C,0x26D3,0x36F2,0x0691,0x16B0,0x6657,0x7676,0x4615,0x5634,
    0xD94C,0xC96D,0xF90E,0xE92F,0x99C8,0x89E9,0xB98A,0xA9AB,0x5844,0x4865,0x7806,0x6827,0x18C0,0x08E1,0x3882,0x28A3,
    0xCB7D,0xDB5C,0xEB3F,0xFB1E,0x8BF9,0x9BD8,0xABBB,0xBB9A,0x4A75,0x5A54,0x6A37,0x7A16,0x0AF1,0x1AD0,0x2AB3,0x3A92,
    0xFD2E,0xED0F,0xDD6C,0xCD4D,0xBDAA,0xAD8B,0x9DE8,0x8DC9,0x7C26,0x6C07,0x5C64,0x4C45,0x3CA2,0x2C83,0x1CE0,0x0CC1,
    0xEF1F,0xFF3E,0xCF5D,0xDF7C,0xAF9B,0xBFBA,0x8FD9,0x9FF8,0x6E17,0x7E36,0x4E55,0x5E74,0x2E93,0x3EB2,0x0ED1,0x1EF0
};

uint16_t IMP::CRC16(const uint8_t* bytes, uint32_t length){
    uint16_t reg = 0x0000;
    for(uint32_t n = 0; n != length; n++){
        reg = ((reg << 8) ^ (__lut_crc16[(uint8_t)((reg >> 8) ^ bytes[n])]));
    }
    return reg;
}

double IMP::GetTimestampUTC(void){
    auto systemClock = std::chrono::system_clock::now();
    std::time_t systemTime = std::chrono::system_clock::to_time_t(systemClock);
    std::tm* gmTime = std::gmtime(&systemTime);
    double s = (double)gmTime->tm_sec;
    double m = (double)gmTime->tm_min;
    double h = (double)gmTime->tm_hour;
    auto duration = systemClock.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= seconds;
    int nanoseconds = (int)std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    return (3600.0 * h + 60.0 * m + s + 1e-9 * double(nanoseconds));
}

double IMP::TimestampDifference(double a, double b){
    double dt = std::fmod(a - b + 86400.0, 86400.0);
    return (dt >= 43200.0) ? (dt - 86400.0) : dt;
}

IMP::InfoRequestMessage::InfoRequestMessage():timestamp(0.0){}

IMP::InfoRequestMessage::~InfoRequestMessage(){}

int IMP::InfoRequestMessage::Encode(uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 8)){
        return -1;
    }

    // ID
    bytes[0] = IMP_ID_INFO_REQUEST;

    // Timestamp
    uint64_t nanosec100 = (uint64_t)CLAMP(10000000.0 * timestamp, 0.0, 1099511627775.0);
    bytes[1] = (uint8_t)(0xFF & (nanosec100 >> 32));
    bytes[2] = (uint8_t)(0xFF & (nanosec100 >> 24));
    bytes[3] = (uint8_t)(0xFF & (nanosec100 >> 16));
    bytes[4] = (uint8_t)(0xFF & (nanosec100 >> 8));
    bytes[5] = (uint8_t)(0xFF & nanosec100);

    // Checksum
    uint16_t crc = IMP::CRC16(&bytes[0], 6);
    bytes[6] = uint8_t(0x00FF & (crc >> 8));
    bytes[7] = uint8_t(0x00FF & crc);
    return 8;
}

int IMP::InfoRequestMessage::Decode(const uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 8)){
        return -1;
    }

    // ID
    if(IMP_ID_INFO_REQUEST != bytes[0]){
        return -1;
    }

    // Timestamp
    timestamp = 0.0000001 * double((uint64_t(bytes[1]) << 32) | (uint64_t(bytes[2]) << 24) | (uint64_t(bytes[3]) << 16) | (uint64_t(bytes[4]) << 8) | uint64_t(bytes[5]));

    // Checksum
    if(IMP::CRC16(&bytes[0], 8)){
        return -1;
    }
    return 8;
}

IMP::InfoResponseMessage::InfoResponseMessage():timestamp(0.0),dimension(std::array<float,3>({0.0f,0.0f,0.0f})),offset(std::array<float,3>({0.0f,0.0f,0.0f})),vehicleName(""),description(""){}

IMP::InfoResponseMessage::~InfoResponseMessage(){}

int IMP::InfoResponseMessage::Encode(uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 30)){
        return -1;
    }
    int index;

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

    // ID
    bytes[0] = IMP_ID_INFO_RESPONSE;

    // Timestamp
    uint64_t nanosec100 = (uint64_t)CLAMP(10000000.0 * timestamp, 0.0, 1099511627775.0);
    bytes[1] = (uint8_t)(0xFF & (nanosec100 >> 32));
    bytes[2] = (uint8_t)(0xFF & (nanosec100 >> 24));
    bytes[3] = (uint8_t)(0xFF & (nanosec100 >> 16));
    bytes[4] = (uint8_t)(0xFF & (nanosec100 >> 8));
    bytes[5] = (uint8_t)(0xFF & nanosec100);

    // Dimension and offset
    index = 6;
    lambdaEncodeFloat(dimension[0]);
    lambdaEncodeFloat(dimension[1]);
    lambdaEncodeFloat(dimension[2]);
    lambdaEncodeFloat(offset[0]);
    lambdaEncodeFloat(offset[1]);
    lambdaEncodeFloat(offset[2]);

    // Text (vehicle name)
    if(((uint32_t)index + (uint32_t)vehicleName.size() + 1) > length){ // vehicleName + zero-terminator (1)
        return -1;
    }
    index += (int)vehicleName.copy((char*)&bytes[index], vehicleName.size());

    // Zero-terminator
    bytes[index++] = 0x00;

    // Text (description)
    if(((uint32_t)index + (uint32_t)description.size() + 3) > length){ // description + zero-terminator (1) + CRC (2)
        return -1;
    }
    index += (int)description.copy((char*)&bytes[index], description.size());

    // Zero-terminator
    bytes[index++] = 0x00;

    // Checksum
    uint16_t crc = IMP::CRC16(&bytes[0], index);
    bytes[index++] = uint8_t(0x00FF & (crc >> 8));
    bytes[index++] = uint8_t(0x00FF & crc);

    // Current index indicates length
    return index;
}

int IMP::InfoResponseMessage::Decode(const uint8_t* bytes, const uint32_t length){
    vehicleName.clear();
    description.clear();
    if(!bytes || (length < 30)){
        return -1;
    }
    int index;

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

    // ID
    if(IMP_ID_INFO_RESPONSE != bytes[0]){
        return -1;
    }

    // Timestamp
    timestamp = 0.0000001 * double((uint64_t(bytes[1]) << 32) | (uint64_t(bytes[2]) << 24) | (uint64_t(bytes[3]) << 16) | (uint64_t(bytes[4]) << 8) | uint64_t(bytes[5]));

    // Dimension, offset
    index = 6;
    dimension[0] = lambdaDecodeFloat();
    dimension[1] = lambdaDecodeFloat();
    dimension[2] = lambdaDecodeFloat();
    offset[0] = lambdaDecodeFloat();
    offset[1] = lambdaDecodeFloat();
    offset[2] = lambdaDecodeFloat();

    // Text (vehicle name) + zero-terminator
    while(index < (int)length){
        uint8_t byte = bytes[index++];
        if(!byte){
            break;
        }
        vehicleName.push_back((char)byte);
    }

    // Text (description) + zero-terminator
    while(index < (int)length){
        uint8_t byte = bytes[index++];
        if(!byte){
            break;
        }
        description.push_back((char)byte);
    }

    // Checksum
    index += 2;
    if((uint32_t(index) > length) || IMP::CRC16(&bytes[0], index)){
        vehicleName.clear();
        description.clear();
        return -1;
    }

    // Current index indicates length
    return index;
}

IMP::GuidanceMessage::GuidanceMessage(){}

IMP::GuidanceMessage::~GuidanceMessage(){}

int IMP::GuidanceMessage::Encode(uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 2)){
        return -1;
    }
    bytes[0] = IMP_ID_GUIDANCE;
    int k = IMP::State::Encode(&bytes[1], length - 1, true);
    if(k < 0){
        return -1;
    }
    k += 3; // three more bytes for ID, CRC(High), CRC(Low)
    if(length < (uint32_t)k){
        return -1;
    }
    uint16_t crc = IMP::CRC16(&bytes[0], k - 2);
    bytes[k-2] = uint8_t(0x00FF & (crc >> 8));
    bytes[k-1] = uint8_t(0x00FF & crc);
    return k;
}

int IMP::GuidanceMessage::Decode(const uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 2)){
        return -1;
    }
    if(IMP_ID_GUIDANCE != bytes[0]){
        return -1;
    }
    int k = IMP::State::Decode(&bytes[1], length - 1, true);
    k += 3; // three more bytes for ID, CRC(High), CRC(Low)
    if(IMP::CRC16(&bytes[0], k)){
        return -1;
    }
    return k;
}

IMP::LogMessage::LogMessage():timestamp(0.0),color(std::array<uint8_t,3>({0,0,0})),text(""){}

IMP::LogMessage::~LogMessage(){}

int IMP::LogMessage::Encode(uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 9)){
        return -1;
    }

    // ID
    bytes[0] = IMP_ID_LOG;

    // Timestamp
    uint64_t nanosec100 = (uint64_t)CLAMP(10000000.0 * timestamp, 0.0, 1099511627775.0);
    bytes[1] = (uint8_t)(0xFF & (nanosec100 >> 32));
    bytes[2] = (uint8_t)(0xFF & (nanosec100 >> 24));
    bytes[3] = (uint8_t)(0xFF & (nanosec100 >> 16));
    bytes[4] = (uint8_t)(0xFF & (nanosec100 >> 8));
    bytes[5] = (uint8_t)(0xFF & nanosec100);

    // Color
    bytes[6] = color[0];
    bytes[7] = color[1];
    bytes[8] = color[2];

    // Text
    int index = 9;
    if(text.size() > 65523){
        text.resize(65523);
    }
    if(((uint32_t)index + (uint32_t)text.size() + 3) > length){ // text + zero-terminator (1) + CRC (2)
        return -1;
    }
    index += (int)text.copy((char*)&bytes[index], text.size());

    // Zero-terminator
    bytes[index++] = 0x00;

    // Checksum
    uint16_t crc = IMP::CRC16(&bytes[0], index);
    bytes[index++] = uint8_t(0x00FF & (crc >> 8));
    bytes[index++] = uint8_t(0x00FF & crc);

    // Current index indicates length
    return index;
}

int IMP::LogMessage::Decode(const uint8_t* bytes, const uint32_t length){
    text.clear();
    if(!bytes || (length < 9)){
        return -1;
    }

    // ID
    if(IMP_ID_LOG != bytes[0]){
        return -1;
    }

    // Timestamp
    timestamp = 0.0000001 * double((uint64_t(bytes[1]) << 32) | (uint64_t(bytes[2]) << 24) | (uint64_t(bytes[3]) << 16) | (uint64_t(bytes[4]) << 8) | uint64_t(bytes[5]));

    // Color
    color[0] = bytes[6];
    color[1] = bytes[7];
    color[2] = bytes[8];

    // Text + zero-terminator
    int index = 9;
    while(index < (int)length){
        uint8_t byte = bytes[index++];
        if(!byte){
            break;
        }
        text.push_back((char)byte);
    }

    // Checksum
    index += 2;
    if((uint32_t(index) > length) || IMP::CRC16(&bytes[0], index)){
        text.clear();
        return -1;
    }

    // Current index indicates length
    return index;
}

IMP::NavigationMessage::NavigationMessage(){}

IMP::NavigationMessage::~NavigationMessage(){}

int IMP::NavigationMessage::Encode(uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 2)){
        return -1;
    }
    bytes[0] = IMP_ID_NAVIGATION;
    int k = IMP::State::Encode(&bytes[1], length - 1, true);
    if(k < 0){
        return -1;
    }
    k += 3; // three more bytes for ID, CRC(High), CRC(Low)
    if(length < (uint32_t)k){
        return -1;
    }
    uint16_t crc = IMP::CRC16(&bytes[0], k - 2);
    bytes[k-2] = uint8_t(0x00FF & (crc >> 8));
    bytes[k-1] = uint8_t(0x00FF & crc);
    return k;
}

int IMP::NavigationMessage::Decode(const uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 2)){
        return -1;
    }
    if(IMP_ID_NAVIGATION != bytes[0]){
        return -1;
    }
    int k = IMP::State::Decode(&bytes[1], length - 1, true);
    k += 3; // three more bytes for ID, CRC(High), CRC(Low)
    if(IMP::CRC16(&bytes[0], k)){
        return -1;
    }
    return k;
}

IMP::ConvexPolygonMessage::ConvexPolygonMessage(): timestamp(0.0), maxSegmentIndex(0), segmentIndex(0), latitude(0.0), longitude(0.0), altitude(0.0){}

IMP::ConvexPolygonMessage::~ConvexPolygonMessage(){}

int IMP::ConvexPolygonMessage::Encode(uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 34)){
        return -1;
    }
    int index;

    // Lambda function for symmetrical range of angles
    auto lambdaSymmetricalRange = [](double d){
        int i = (int)(d * 0.159154943091895);
        d -= 6.28318530717959 * (double)i;
        return (d < -3.14159265358979) ? (d + 6.28318530717959) : ((d >= 3.14159265358979) ? (d - 6.28318530717959) : d);
    };

    // Lambda functions for data type encoding
    auto lambdaEncodeDouble = [&bytes, &index](double value){
        union {
            double d;
            uint8_t b[8];
        } un;
        un.d = value;
        if(1 != IMPSwapByteOrder(1)){
            uint8_t h = un.b[0]; un.b[0] = un.b[7]; un.b[7] = h;
            h = un.b[1]; un.b[1] = un.b[6]; un.b[6] = h;
            h = un.b[2]; un.b[2] = un.b[5]; un.b[5] = h;
            h = un.b[3]; un.b[3] = un.b[4]; un.b[4] = h;
        }
        memcpy(&bytes[index], &un.b[0], 8);
        index += 8;
    };

    // ID
    bytes[0] = IMP_ID_CONVEX_POLYGON;

    // Timestamp
    uint64_t nanosec100 = (uint64_t)CLAMP(10000000.0 * timestamp, 0.0, 1099511627775.0);
    bytes[1] = (uint8_t)(0xFF & (nanosec100 >> 32));
    bytes[2] = (uint8_t)(0xFF & (nanosec100 >> 24));
    bytes[3] = (uint8_t)(0xFF & (nanosec100 >> 16));
    bytes[4] = (uint8_t)(0xFF & (nanosec100 >> 8));
    bytes[5] = (uint8_t)(0xFF & nanosec100);

    // Maximum segment index
    bytes[6] = maxSegmentIndex;

    // Segment index
    bytes[7] = segmentIndex;

    // Origin (lat, lon)
    index = 8;
    lambdaEncodeDouble(lambdaSymmetricalRange(latitude));
    lambdaEncodeDouble(lambdaSymmetricalRange(longitude));
    lambdaEncodeDouble(altitude);

    // Number of polygons
    if(polygons.size() > 65535){
        polygons.resize(65535);
    }
    uint32_t numPolygons = 0x0000FFFF & uint32_t(polygons.size());
    bytes[index++] = (uint8_t)(0xFF & (numPolygons >> 8));
    bytes[index++] = (uint8_t)(0xFF & numPolygons);

    // Polygon data
    int remainingLength = (int)length - index;
    for(uint32_t p = 0; p < numPolygons; p++){
        if(remainingLength < 1){
            return -1;
        }
        int written = polygons[p].Encode(&bytes[index], (uint32_t)remainingLength);
        if(written < 0){
            return -1;
        }
        index += written;
        remainingLength -= written; 
    }

    // Checksum
    if(uint32_t(index + 2) > length){
        return -1;
    }
    uint16_t crc = IMP::CRC16(&bytes[0], index);
    bytes[index++] = uint8_t(0x00FF & (crc >> 8));
    bytes[index++] = uint8_t(0x00FF & crc);

    // Current index indicates length
    return index;
}

int IMP::ConvexPolygonMessage::Decode(const uint8_t* bytes, const uint32_t length){
    polygons.clear();
    if(!bytes || (length < 34)){
        return -1;
    }
    int index;

    // Lambda function for symmetrical range of angles
    auto lambdaSymmetricalRange = [](double d){
        int i = (int)(d * 0.159154943091895);
        d -= 6.28318530717959 * (double)i;
        return (d < -3.14159265358979) ? (d + 6.28318530717959) : ((d >= 3.14159265358979) ? (d - 6.28318530717959) : d);
    };

    // Lambda functions for data type decoding
    auto lambdaDecodeDouble = [&bytes, &index](){
        union {
            double d;
            uint8_t b[8];
        } un;
        memcpy(&un.b[0], &bytes[index], 8);
        if(1 != IMPSwapByteOrder(1)){
            uint8_t h = un.b[0]; un.b[0] = un.b[7]; un.b[7] = h;
            h = un.b[1]; un.b[1] = un.b[6]; un.b[6] = h;
            h = un.b[2]; un.b[2] = un.b[5]; un.b[5] = h;
            h = un.b[3]; un.b[3] = un.b[4]; un.b[4] = h;
        }
        index += 8;
        return un.d;
    };

    // ID
    if(IMP_ID_CONVEX_POLYGON != bytes[0]){
        return -1;
    }

    // Timestamp
    timestamp = 0.0000001 * double((uint64_t(bytes[1]) << 32) | (uint64_t(bytes[2]) << 24) | (uint64_t(bytes[3]) << 16) | (uint64_t(bytes[4]) << 8) | uint64_t(bytes[5]));

    // Maximum segment index
    maxSegmentIndex = bytes[6];

    // Segment index
    segmentIndex = bytes[7];

    // Origin (lat, lon)
    index = 8;
    latitude = lambdaSymmetricalRange(lambdaDecodeDouble());
    longitude = lambdaSymmetricalRange(lambdaDecodeDouble());
    altitude = lambdaDecodeDouble();

    // Number of polygons
    uint32_t numPolygons = (uint32_t(bytes[index]) << 8) | uint32_t(bytes[index + 1]);
    index += 2;

    // Polygon data
    int remainingLength = (int)length - index;
    for(uint32_t p = 0; p < numPolygons; p++){
        if(remainingLength < 1){
            polygons.clear();
            return -1;
        }
        polygons.push_back(ConvexPolygon());
        int read = polygons.back().Decode(&bytes[index], (uint32_t)remainingLength);
        if(read < 0){
            polygons.clear();
            return -1;
        }
        index += read;
        remainingLength -= read;
    }

    // Checksum
    index += 2;
    if((uint32_t(index) > length) || IMP::CRC16(&bytes[0], index)){
        polygons.clear();
        return -1;
    }

    // Current index indicates length
    return index;
}

uint32_t IMP::ConvexPolygonMessage::NumberOfPolygonsFitIntoMessage(const std::vector<ConvexPolygon>& polygons, const uint32_t maxMessageSize){
    // Overhead: ID(1) + timestamp(5) + maxSegmentIndex(1) + segmentIndex(1) + originLLA(24) + numPolygons(2) + CRC(2)
    uint32_t length = 1 + 5 + 1 + 1 + 24 + 2 + 2;
    if(length > maxMessageSize){
        return 0;
    }

    // Increment length for each polygon (at most 65535)
    uint32_t result = 0;
    size_t nmax = (polygons.size() > 65535) ? 65535 : polygons.size();
    for(size_t n = 0; n < nmax; n++){
        // Polygon overhead: polygonID(1) + classification(1) + numVertices(1)
        uint32_t requiredLength = 3;

        // If classification indicates a dynamic polygon additional bytes for velocities are required (12)
        requiredLength += 12 * (uint32_t)((0x80 & polygons[n].classification) >> 7);

        // Each vertex (at most 255) requires 8 bytes
        requiredLength += 8 * (uint32_t)((polygons[n].vertices.size() > 255) ? 255 : polygons[n].vertices.size());

        // Break if required length does not fit
        if((length + requiredLength) > maxMessageSize){
            break;
        }

        // Polygon fits, increment
        result++;
        length += requiredLength;
    }
    return result;
}

IMP::GuidanceTrajectoryMessage::GuidanceTrajectoryMessage():timestamp(0.0),startTimeYearUTC(0),startTimeMonthUTC(0),startTimeDayUTC(0),startTimeSecondsUTC(0.0),deltaTime(0.0){}

IMP::GuidanceTrajectoryMessage::~GuidanceTrajectoryMessage(){}

int IMP::GuidanceTrajectoryMessage::Encode(uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 24)){
        return -1;
    }
    int index;

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

    // ID
    bytes[0] = IMP_ID_GUIDANCE_TRAJECTORY;

    // Timestamp
    uint64_t nanosec100 = (uint64_t)CLAMP(10000000.0 * timestamp, 0.0, 1099511627775.0);
    bytes[1] = (uint8_t)(0xFF & (nanosec100 >> 32));
    bytes[2] = (uint8_t)(0xFF & (nanosec100 >> 24));
    bytes[3] = (uint8_t)(0xFF & (nanosec100 >> 16));
    bytes[4] = (uint8_t)(0xFF & (nanosec100 >> 8));
    bytes[5] = (uint8_t)(0xFF & nanosec100);

    // Start time (year (15 bit), month (4 bit), day (5 bit))
    uint32_t year = (uint32_t)(startTimeYearUTC = CLAMP(startTimeYearUTC, 0, 32767));
    uint32_t month = (uint32_t)(startTimeMonthUTC = CLAMP(startTimeMonthUTC, 1, 12));
    uint32_t day = (uint32_t)(startTimeDayUTC = CLAMP(startTimeDayUTC, 1, 31));
    bytes[6] = (uint8_t)(0xFF & (year >> 7));
    bytes[7] = (uint8_t)(0xFE & (year << 1)) | (uint8_t)(0x01 & (month >> 3));
    bytes[8] = (uint8_t)(0xE0 & (month << 5)) | (uint8_t)(0x1F & day);

    // Start time (UTC seconds of day)
    nanosec100 = (uint64_t)CLAMP(10000000.0 * startTimeSecondsUTC, 0.0, 1099511627775.0);
    bytes[9] = (uint8_t)(0xFF & (nanosec100 >> 32));
    bytes[10] = (uint8_t)(0xFF & (nanosec100 >> 24));
    bytes[11] = (uint8_t)(0xFF & (nanosec100 >> 16));
    bytes[12] = (uint8_t)(0xFF & (nanosec100 >> 8));
    bytes[13] = (uint8_t)(0xFF & nanosec100);

    // Delta time
    index = 14;
    deltaTime = (deltaTime < 0.0) ? 0.0 : deltaTime;
    lambdaEncodeFloat(deltaTime);

    // Number of points
    if(points.size() > 65535){
        points.resize(65535);
    }
    uint16_t numPoints = (uint16_t)points.size();
    bytes[index++] = (uint8_t)(0xFF & (numPoints >> 8));
    bytes[index++] = (uint8_t)(0xFF & numPoints);

    // Configuration
    int written = configuration.Encode(&bytes[index], 4);
    if(written < 0){
        return -1;
    }
    index += written;

    // Points
    int remainingLength = (int)length - index;
    for(uint16_t n = 0; n < numPoints; n++){
        if(remainingLength < 1){
            return -1;
        }
        points[n].configuration = configuration;
        written = points[n].Encode(&bytes[index], (uint32_t)remainingLength, false);
        if(written < 0){
            return -1;
        }
        index += written;
        remainingLength -= written;
    }

    // Checksum
    if(uint32_t(index + 2) > length){
        return -1;
    }
    uint16_t crc = IMP::CRC16(&bytes[0], index);
    bytes[index++] = uint8_t(0x00FF & (crc >> 8));
    bytes[index++] = uint8_t(0x00FF & crc);

    // Current index indicates length
    return index;
}

int IMP::GuidanceTrajectoryMessage::Decode(const uint8_t* bytes, const uint32_t length){
    points.clear();
    if(!bytes || (length < 24)){
        return -1;
    }
    int index;

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

    // ID
    if(IMP_ID_GUIDANCE_TRAJECTORY != bytes[0]){
        return -1;
    }

    // Timestamp
    timestamp = 0.0000001 * double((uint64_t(bytes[1]) << 32) | (uint64_t(bytes[2]) << 24) | (uint64_t(bytes[3]) << 16) | (uint64_t(bytes[4]) << 8) | uint64_t(bytes[5]));

    // Start time (year (15 bit), month (4 bit), day (5 bit))
    startTimeYearUTC = (int32_t)(((uint32_t)(bytes[6]) << 7) | (uint32_t)(bytes[7] >> 1));
    startTimeMonthUTC = (int32_t)(((uint32_t)(0x01 & bytes[7]) << 3) | ((uint32_t)(0xE0 & bytes[8]) >> 5));
    startTimeDayUTC = (int32_t)((uint32_t)(0x1F & bytes[8]));

    // Start time (UTC seconds of day)
    startTimeSecondsUTC = 0.0000001 * double((uint64_t(bytes[9]) << 32) | (uint64_t(bytes[10]) << 24) | (uint64_t(bytes[11]) << 16) | (uint64_t(bytes[12]) << 8) | uint64_t(bytes[13]));

    // Delta time
    index = 14;
    deltaTime = lambdaDecodeFloat();

    // Number of points
    uint16_t numPoints = (uint16_t(bytes[index]) << 8) | uint16_t(bytes[index + 1]);
    index += 2;

    // Configuration
    int read = configuration.Decode(&bytes[index], 4);
    if(read < 0){
        return -1;
    }
    index += read;

    // Points
    int remainingLength = (int)length - index;
    for(uint16_t n = 0; n < numPoints; n++){
        if(remainingLength < 1){
            points.clear();
            return -1;
        }
        points.push_back(State());
        points.back().configuration = configuration;
        read = points.back().Decode(&bytes[index], (uint32_t)remainingLength, false);
        if(read < 0){
            points.clear();
            return -1;
        }
        index += read;
        remainingLength -= read;
    }

    // Checksum
    index += 2;
    if((uint32_t(index) > length) || IMP::CRC16(&bytes[0], index)){
        points.clear();
        return -1;
    }

    // Current index indicates length
    return index;
}

void IMP::GuidanceTrajectoryMessage::SetStartTimeNow(void){
    auto systemClock = std::chrono::system_clock::now();
    std::time_t systemTime = std::chrono::system_clock::to_time_t(systemClock);
    std::tm* gmTime = std::gmtime(&systemTime);
    double s = (double)gmTime->tm_sec;
    double m = (double)gmTime->tm_min;
    double h = (double)gmTime->tm_hour;
    auto duration = systemClock.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= seconds;
    int nanoseconds = (int)std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    this->startTimeYearUTC = (int32_t)gmTime->tm_year;
    this->startTimeMonthUTC = (int32_t)gmTime->tm_mon;
    this->startTimeDayUTC = (int32_t)gmTime->tm_mday;
    this->startTimeSecondsUTC = (3600.0 * h + 60.0 * m + s + 1e-9 * double(nanoseconds));
}

