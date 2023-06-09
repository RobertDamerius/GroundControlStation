#include <Common.hpp>
#include <AppWindow.hpp>
#include <VehicleID.hpp>


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Version Settings
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const std::string strAppName("GroundControlStation");
const std::string strVersion("20230610");
const std::string strBuilt(__DATE__ " " __TIME__);


double GetTimestampUTC(void){
    auto timePoint = std::chrono::high_resolution_clock::now();
    std::time_t systemTime = std::chrono::high_resolution_clock::to_time_t(timePoint);
    std::tm* gmTime = std::gmtime(&systemTime);
    double s = (double)gmTime->tm_sec;
    double m = (double)gmTime->tm_min;
    double h = (double)gmTime->tm_hour;
    auto duration = timePoint.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= seconds;
    int nanoseconds = (int)std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    return (3600.0 * h + 60.0 * m + s + 1e-9 * double(nanoseconds));
}

void GUILog(std::string msg, uint8_t r, uint8_t g, uint8_t b){
    appWindow.canvas.gui.AddLogEntry(LogEntry(GetTimestampUTC(), VehicleID(strAppName), msg, r, g, b));
}

