/**
 *  @file CFGParser.hpp
 *  @details Version 20200920.
 */
#pragma once


/**
 *  @brief Class: CFGParser
 *  @details Parse a configuration file line by line. An example configuration file looks as follows:
 *
 *      # This is a comment
 *      [KEY_NAME]
 *          VALUE_NAME
 *          VALUE_NAME
 *
 *  Only printable ASCII characters are allowed. Everything below 0x20 and above 0x7E will be removed. Tabs will be
 *  replaced by spaces. A value string must occur after a key string. A key can contain a various amount of values (or even no value).
 *  More values can be assigned to a key as shown above. Comment lines should begin with the '#' character.
 */
class CFGParser {
    public:
        std::unordered_map<std::string, std::vector<std::string>> configuration;

        /**
         *  @brief Parse a configuration file.
         *  @param [in] filename The name of the file that should be parsed.
         *  @return 0 if success or the line number at which an error occurred. If file does not exist 0xFFFFFFFF is returned.
         */
        uint32_t Parse(std::string filename);

        /**
         *  @brief Find the value for a given key for which only one key is allowed.
         *  @param [in] out The value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if found, false otherwise. If the value has an invalid format, false is returned.
         *  @details The value must be either "true" or "false". All characters can be either upper or lower case.
         */
        bool FindSingleValue(bool& out, const char* key);

        /**
         *  @brief Find the value for a given key for which only one key is allowed.
         *  @param [in] out The value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if found, false otherwise. If the value has an invalid format, false is returned.
         */
        bool FindSingleValue(int32_t& out, const char* key);

        /**
         *  @brief Find the value for a given key for which only one key is allowed.
         *  @param [in] out The value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if found, false otherwise. If the value has an invalid format, false is returned.
         */
        bool FindSingleValue(uint32_t& out, const char* key);

        /**
         *  @brief Find the value for a given key for which only one key is allowed.
         *  @param [in] out The value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if found, false otherwise. If the value has an invalid format, false is returned.
         */
        bool FindSingleValue(double& out, const char* key);

        /**
         *  @brief Find the value for a given key for which only one key is allowed.
         *  @param [in] out The value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if found, false otherwise. If the value has an invalid format, false is returned.
         *  @details The value must contain three integer values separated by comma, e.g "1,2,-3".
         */
        bool FindSingleValue(std::array<int32_t,3>& out, const char* key);

        /**
         *  @brief Find the value for a given key for which only one key is allowed.
         *  @param [in] out The value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if found, false otherwise. If the value has an invalid format, false is returned.
         *  @details The value must contain three signed values separated by comma, e.g "1,2,3".
         */
        bool FindSingleValue(std::array<uint32_t,3>& out, const char* key);

        /**
         *  @brief Find the value for a given key for which only one key is allowed.
         *  @param [in] out The value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if found, false otherwise. If the value has an invalid format, false is returned.
         *  @details The value must contain three double values separated by comma, e.g "1.0,2.0,-3.0".
         */
        bool FindSingleValue(std::array<double,3>& out, const char* key);

}; /* class: CFGParser */

