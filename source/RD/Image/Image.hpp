/**
 *  @file Image.hpp
 *  @brief The image header.
 *  @details Version 20200921.
 */
#pragma once


/* Default module namespace */
namespace RD {
namespace Image {


class PNG {
    public:
        uint32_t width;             ///< Image width in pixels.
        uint32_t height;            ///< Image height in pixels.
        uint32_t numChannels;       ///< Number of channels, e.g. 3 for RGB.
        std::vector<uint8_t> data;  ///< Raw pixel data.

        /**
         *  @brief Create an empty PNG image object.
         */
        PNG();

        /**
         *  @brief Read PNG image from file.
         *  @param [in] filename Name of the PNG file.
         *  @param [in] numChannels Number of channels to be read, either 3 (RGB) or 4 (RGBA).
         *  @param [in] mirror True if rows should be swapped.
         *  @return True if success, false otherwise.
         */
        bool Read(std::string filename, uint32_t numChannels, bool mirror = true);

        /**
         *  @brief Read PNG image from binary data.
         *  @param [in] bytes Binary data.
         *  @param [in] numChannels Number of channels to be read, either 3 (RGB) or 4 (RGBA).
         *  @param [in] mirror True if rows should be swapped.
         *  @return True if success, false otherwise.
         */
        bool Read(std::vector<uint8_t>& bytes, uint32_t numChannels, bool mirror = true);

        /**
         *  @brief Write PNG image to file.
         *  @param [in] filename Name of the PNG file.
         *  @param [in] mirror True if rows should be swapped.
         *  @return True if success, false otherwise.
         *  @details @ref width, @ref height, @ref numChannels and the size of @ref data must match, otherwise false is returned. The PNG file will always be stored as RGBA image.
         */
        bool Write(std::string filename, bool mirror = true);

        /**
         *  @brief Write PNG image to binary data.
         *  @param [out] bytes Container where to store binary data.
         *  @param [in] mirror True if rows should be swapped.
         *  @return True if success, false otherwise.
         *  @details @ref width, @ref height, @ref numChannels and the size of @ref data must match, otherwise false is returned. The PNG file will always be stored as RGBA image.
         */
        bool Write(std::vector<uint8_t>& bytes, bool mirror = true);

}; /* class: PNG */


} /* namespace: Image */
} /* namespace: RD */

