/**
 *  @file InterModuleProtocol.hpp
 *  @brief Version 20200919.
 *  @details This is the main header file. You only need to include this header file to use the Inter-Module Protocol.
 */
#pragma once


#include <IMPCommon.hpp>
#include <IMPState.hpp>
#include <IMPStateConfiguration.hpp>
#include <IMPConvexPolygon.hpp>


/* Inter-Module Protocol ID */
#define IMP_ID_INFO_REQUEST         (0x00)   ///< ID for info request message.
#define IMP_ID_INFO_RESPONSE        (0x01)   ///< ID for info response message.
#define IMP_ID_GUIDANCE             (0x47)   ///< 'G': ID for guidance message.
#define IMP_ID_LOG                  (0x4C)   ///< 'L': ID for log message.
#define IMP_ID_NAVIGATION           (0x4E)   ///< 'N': ID for navigation message.
#define IMP_ID_CONVEX_POLYGON       (0x50)   ///< 'P': ID for convex polygon message.
#define IMP_ID_GUIDANCE_TRAJECTORY  (0x54)   ///< 'T': ID for guidance trajectory.


/* Default namespace */
namespace IMP {


class InfoRequestMessage {
    public:
        /* Header */
        double timestamp;     ///< UTC Timestamp: seconds of the day.

        /**
         *  @brief Create an info request message object.
         */
        InfoRequestMessage();

        /**
         *  @brief Delete the info request message object.
         */
        ~InfoRequestMessage();

        /**
         *  @brief Encode the info request message.
         *  @param [out] bytes Output buffer where to store the message.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be written to the buffer).
         *  @returns The number of bytes that have been written to the buffer or -1 in case of failure.
         */
        int Encode(uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Decode the info request message.
         *  @param [in] bytes Input buffer containing the message to be decoded.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be read from the buffer).
         *  @return Number of bytes that have been used for decoding or -1 in case of failure.
         */
        int Decode(const uint8_t* bytes, const uint32_t length);
};


class InfoResponseMessage {
    public:
        /* Header */
        double timestamp;                ///< UTC Timestamp: seconds of the day.

        /* Data */
        std::array<float, 3> dimension;  ///< Dimension of the default vehicle cuboid in meters (body-frame).
        std::array<float, 3> offset;     ///< Offset from body-frame origin to cuboid in meters (body-frame).
        std::string vehicleName;         ///< Name of the vehicle to which the module belongs to.
        std::string description;         ///< Optional description string.

        /**
         *  @brief Create an info response message object.
         */
        InfoResponseMessage();

        /**
         *  @brief Delete the info response message object.
         */
        ~InfoResponseMessage();

        /**
         *  @brief Encode the info response message.
         *  @param [out] bytes Output buffer where to store the message.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be written to the buffer).
         *  @returns The number of bytes that have been written to the buffer or -1 in case of failure.
         */
        int Encode(uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Decode the info response message.
         *  @param [in] bytes Input buffer containing the message to be decoded.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be read from the buffer).
         *  @return Number of bytes that have been used for decoding or -1 in case of failure.
         */
        int Decode(const uint8_t* bytes, const uint32_t length);
};


class GuidanceMessage: public State {
    public:
        /**
         *  @brief Create a guidance message object.
         */
        GuidanceMessage();

        /**
         *  @brief Delete the guidance message object.
         */
        ~GuidanceMessage();

        /**
         *  @brief Encode the guidance message from the guidance state. The packet length depends on the configuration.
         *  @param [out] bytes Output buffer where to store the guidance message.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be written to the buffer).
         *  @returns The number of bytes that have been written to the buffer or -1 in case of failure.
         *  @details The minimum number of bytes is 12, maximum number of bytes is 268.
         */
        int Encode(uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Decode the guidance message to the guidance state.
         *  @param [in] bytes Input buffer containing the message to be decoded.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be read from the buffer).
         *  @return Number of bytes that have been used for decoding or -1 in case of failure.
         */
        int Decode(const uint8_t* bytes, const uint32_t length);

    private:
        using State::Encode;
        using State::Decode;
};


class LogMessage {
    public:
        /* Header */
        double timestamp;              ///< UTC Timestamp: seconds of the day.
        std::array<uint8_t, 3> color;  ///< Color for the log text.

        /* Data */
        std::string text;              ///< The actual log text.

        /**
         *  @brief Create a log message object.
         */
        LogMessage();

        /**
         *  @brief Delete the log message object.
         */
        ~LogMessage();

        /**
         *  @brief Encode the log message. The packet length depends on length of the @ref text.
         *  @param [out] bytes Output buffer where to store the log message.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be written to the buffer).
         *  @returns The number of bytes that have been written to the buffer or -1 in case of failure.
         *  @details If @ref text contains more than 65523 characters then the container will be truncated.
         *  @note Note, that the maximum number of bytes that can be send via UDP is 65535 bytes!
         */
        int Encode(uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Decode the log message.
         *  @param [in] bytes Input buffer containing the message to be decoded.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be read from the buffer).
         *  @return Number of bytes that have been used for decoding or -1 in case of failure.
         */
        int Decode(const uint8_t* bytes, const uint32_t length);
};


class NavigationMessage: public State {
    public:
        /**
         *  @brief Create a navigation message object.
         */
        NavigationMessage();

        /**
         *  @brief Delete the navigation message object.
         */
        ~NavigationMessage();

        /**
         *  @brief Encode the navigation message from the navigation state. The packet length depends on the configuration.
         *  @param [out] bytes Output buffer where to store the navigation message.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be written to the buffer).
         *  @returns The number of bytes that have been written to the buffer or -1 in case of failure.
         *  @details The minimum number of bytes is 12, maximum number of bytes is 268.
         */
        int Encode(uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Decode the navigation message to the navigation state.
         *  @param [in] bytes Input buffer containing the message to be decoded.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be read from the buffer).
         *  @return Number of bytes that have been used for decoding or -1 in case of failure.
         */
        int Decode(const uint8_t* bytes, const uint32_t length);

    private:
        using State::Encode;
        using State::Decode;
};


class ConvexPolygonMessage {
    public:
        /* Header */
        double timestamp;        ///< UTC Timestamp: seconds of the day.
        uint8_t maxSegmentIndex; ///< The maximum segment index number. That is the maximum number of polgon messages belonging to a polygon dataset is (maxSegmentIndex + 1).
        uint8_t segmentIndex;    ///< The current segment index number for this message.
        double latitude;         ///< Latitude of the origin in radians.
        double longitude;        ///< Longitude of the origin in radians.
        double altitude;         ///< Altitude of the origin in meters.

        /* Data */
        std::vector<ConvexPolygon> polygons; ///< List of convex polygons.

        /**
         *  @brief Create a convex polygon message object.
         */
        ConvexPolygonMessage();

        /**
         *  @brief Delete the convex polygon message object.
         */
        ~ConvexPolygonMessage();

        /**
         *  @brief Encode the convex polygon message. The packet length depends on the number of @ref polygons.
         *  @param [out] bytes Output buffer where to store the polygon message.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be written to the buffer).
         *  @returns The number of bytes that have been written to the buffer or -1 in case of failure.
         *  @details The maximum number of polygons is limited to 65535 (2^16 - 1). The maximum number of vertices per polygon is limited to 255 (2^8 - 1).
         *  If @ref polygons contains more polygons/vertices then the containers are truncated.
         *  @note Note, that the maximum number of bytes that can be send via UDP is 65535 bytes!
         */
        int Encode(uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Decode the convex polygon message.
         *  @param [in] bytes Input buffer containing the message to be decoded.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be read from the buffer).
         *  @return Number of bytes that have been used for decoding or -1 in case of failure.
         */
        int Decode(const uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Get the number of polygons from a polygon set that fit into a message with a specified message size.
         *  @param [in] polygons The set of polygons that should be tried to fit into a message.
         *  @param [in] maxMessageSize The maximum number of bytes that can be used for a message.
         *  @return The number of polygons that fit into the message.
         *  @details The protocol overhead (ID, timestamp, ..., CRC) is taken into account.
         */
        static uint32_t NumberOfPolygonsFitIntoMessage(const std::vector<ConvexPolygon>& polygons, const uint32_t maxMessageSize);
};


class GuidanceTrajectoryMessage {
    public:
        /* Header */
        double timestamp;                  ///< UTC Timestamp: seconds of the day.
        int32_t startTimeYearUTC;          ///< Start time for the trajectory: year (UTC).
        int32_t startTimeMonthUTC;         ///< Start time for the trajectory: month (UTC), range [1, 12].
        int32_t startTimeDayUTC;           ///< Start time for the trajectory: day (UTC), range [1, 31].
        double startTimeSecondsUTC;        ///< Start time for the trajectory: seconds of the day (UTC).
        double deltaTime;                  ///< Time difference from one state point to another in seconds.
        StateConfiguration configuration;  ///< The configuration for the state data points.

        /* Data */
        std::vector<State> points;

        /**
         *  @brief Create a guidance trajectory message object.
         */
        GuidanceTrajectoryMessage();

        /**
         *  @brief Delete the guidance trajectory message object.
         */
        ~GuidanceTrajectoryMessage();

        /**
         *  @brief Encode the guidance trajectory message. The packet length depends on the number of @ref points.
         *  @param [out] bytes Output buffer where to store the message.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be written to the buffer).
         *  @returns The number of bytes that have been written to the buffer or -1 in case of failure.
         */
        int Encode(uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Decode the guidance trajectory message.
         *  @param [in] bytes Input buffer containing the message to be decoded.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be read from the buffer).
         *  @return Number of bytes that have been used for decoding or -1 in case of failure.
         */
        int Decode(const uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Set the start time (@ref startTimeYearUTC, @ref startTimeMonthUTC, @ref startTimeDayUTC and @ref startTimeSecondsUTC) based on current system time.
         */
        void SetStartTimeNow(void);
};


/**
 *  @brief Get the current timestamp.
 *  @returns The UTC timestamp that indicates the seconds of the day.
 */
double GetTimestampUTC(void);


/**
 *  @brief Calculate the time difference of two UTC timestamp.
 *  @param [in] a UTC timestamp 1 (UTC seconds of the day).
 *  @param [in] b UTC timestamp 2 (UTC seconds of the day).
 *  @return The time difference "a - b" in seconds in range [-43200.0, 43200.0).
 */
double TimestampDifference(double a, double b);


/**
 *  @brief Calculate the 16-bit CRC.
 *  @param [in] bytes Input message.
 *  @param [in] length Length of the input message.
 *  @return 16-bit CRC value.
 *  @details A CCIT_ZERO checksum is used (Polynomial: 0x1021, initial value: 0x0000, Final Xor Value: 0x0000, no reflected input, no reflected result).
 */
uint16_t CRC16(const uint8_t* bytes, uint32_t length);


} /* namespace: IMP */

