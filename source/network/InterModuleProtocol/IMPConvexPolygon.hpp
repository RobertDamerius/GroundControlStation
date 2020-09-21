/**
 *  @file IMPConvexPolygon.hpp
 *  @brief Version 20200919.
 */
#pragma once


#include <IMPCommon.hpp>


/* Default namespace */
namespace IMP {


class ConvexPolygon {
    public:
        uint16_t polygonID;                          ///< The unique polygon ID.
        uint8_t classification;                      ///< Classification byte. Static polygon: [0, 127], dynamic polygon: [128, 255].
        double velocityNorth;                        ///< Velocity in north direction in meters per second.
        double velocityEast;                         ///< Velocity in east direction in meters per second.
        double yawRate;                              ///< Rotational velocity around downward axis of the navigation frame in radians per second.
        std::vector<std::array<double, 2>> vertices; ///< The vertices given in mathematically positive order. Each vertex has an x- and a y-coordinate in meters that relates to the originLLA of the ConvexPolygonMessage.

        /**
         *  @brief Create a convex polygon object.
         */
        ConvexPolygon();

        /**
         *  @brief Delete the convex polygon object.
         */
        ~ConvexPolygon();

        /**
         *  @brief Encode the convex polygon to a buffer.
         *  @param [out] bytes Output buffer where to store the bytes.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be written to the buffer).
         *  @returns The number of bytes that have been written to the buffer or -1 in case of failure.
         *  @note The number of vertices will be truncated to 255.
         */
        int Encode(uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Decode a convex polygon from a buffer.
         *  @param [in] bytes Input buffer containing the convex polygon to be decoded.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be read from the buffer).
         *  @return Number of bytes that have been decoded or -1 in case of failure.
         */
        int Decode(const uint8_t* bytes, const uint32_t length);
};


} /* namespace: IMP */

