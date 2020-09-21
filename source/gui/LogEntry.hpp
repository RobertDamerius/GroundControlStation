#pragma once


#include <VehicleID.hpp>


class LogEntry {
    public:
        std::string timestamp;
        std::string source;
        std::string text;
        uint8_t color[3];

        /**
         * @brief Create a log entry.
         * @param [in] timestamp The UTC second of the day.
         * @param [in] source Source of the log message.
         * @param [in] text Text of the log message.
         * @param [in] r Red component for text color.
         * @param [in] g Green component for text color.
         * @param [in] b Blue component for text color.
         */
        LogEntry(double timestamp, const VehicleID& source, std::string& text, uint8_t r, uint8_t g, uint8_t b);

        /**
         * @brief Delete the log entry.
         */
        ~LogEntry();
};

