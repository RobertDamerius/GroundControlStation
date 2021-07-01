#include <InterModuleProtocol.hpp>


#define CLAMP(x, lo, hi) ((x < lo) ? lo : ((x > hi) ? hi : x))


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
    if(!bytes || (length < 6)){
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
    return 6;
}

int IMP::InfoRequestMessage::Decode(const uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 6)){
        return -1;
    }

    // ID
    if(IMP_ID_INFO_REQUEST != bytes[0]){
        return -1;
    }

    // Timestamp
    timestamp = 0.0000001 * double((uint64_t(bytes[1]) << 32) | (uint64_t(bytes[2]) << 24) | (uint64_t(bytes[3]) << 16) | (uint64_t(bytes[4]) << 8) | uint64_t(bytes[5]));
    return 6;
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

    // Current index indicates length
    return index;
}

IMP::GuidanceMessage::GuidanceMessage(){}

IMP::GuidanceMessage::~GuidanceMessage(){}

int IMP::GuidanceMessage::Encode(uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 1)){
        return -1;
    }
    bytes[0] = IMP_ID_GUIDANCE;
    int k = IMP::State::Encode(&bytes[1], length - 1, true);
    if(k < 0){
        return -1;
    }
    k += 1; // one more byte for ID
    if(length < (uint32_t)k){
        return -1;
    }
    return k;
}

int IMP::GuidanceMessage::Decode(const uint8_t* bytes, const uint32_t length){
    if(!bytes || (length < 1)){
        return -1;
    }
    if(IMP_ID_GUIDANCE != bytes[0]){
        return -1;
    }
    int k = IMP::State::Decode(&bytes[1], length - 1, true);
    if(k < 0){
        return -1;
    }
    return k + 1; // one more byte for ID
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
    if(((uint32_t)index + (uint32_t)text.size() + 1) > length){ // text + zero-terminator (1)
        return -1;
    }
    index += (int)text.copy((char*)&bytes[index], text.size());

    // Zero-terminator
    bytes[index++] = 0x00;

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
    k += 1; // one more byte for ID
    if(length < (uint32_t)k){
        return -1;
    }
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
    if(k < 0){
        return -1;
    }
    return k + 1; // one more byte for ID
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

    // Current index indicates length
    return index;
}

uint32_t IMP::ConvexPolygonMessage::NumberOfPolygonsFitIntoMessage(const std::vector<ConvexPolygon>& polygons, const uint32_t maxMessageSize){
    // Overhead: ID(1) + timestamp(5) + maxSegmentIndex(1) + segmentIndex(1) + originLLA(24) + numPolygons(2)
    uint32_t length = 1 + 5 + 1 + 1 + 24 + 2;
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

