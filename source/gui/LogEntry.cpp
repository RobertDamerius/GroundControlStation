#include <LogEntry.hpp>


LogEntry::LogEntry(double timestamp, const VehicleID& source, std::string& text, uint8_t r, uint8_t g, uint8_t b){
    // Timestamp [HH:MM:SS]
    double hours = timestamp / 3600.0;
    int h = (int)hours;
    double minutes = (hours - (double)h) * 60.0;
    int m = (int)minutes;
    int s = (int)((minutes - (double)m) * 60.0);
    char buf[32];
    sprintf(&buf[0], "[%02d:%02d:%02d]",h,m,s);
    this->timestamp = std::string(buf);

    // Source (vehicle name)
    this->source = source.name;

    // Text
    this->text = text;

    // Color
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

LogEntry::~LogEntry(){}

