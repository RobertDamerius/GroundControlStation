/**
 *  @file Core.hpp
 *  @brief The core header file.
 *  @details Version 20210203.
 */
#pragma once


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Macros to load included binary resource files
// MinGW: Create a resource object file with "ld -r -b binary -o data.o data.bin"
// You can view segments with "objdump -t data.o"
// Add data.o as source to the linker step. In your code you once call
// RESOURCE_EXTLD(data_bin) outside. The size and the data bytes can be
// obtained by:
// size_t len = RESOURCE_LDLEN(data_bin);
// const unsigned char* data = RESOURCE_LDVAR(data_bin);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef __APPLE__
#include <mach-o/getsect.h>
#define RESOURCE_EXTLD(NAME) \
  extern const unsigned char _section$__DATA__ ## NAME [];
#define RESOURCE_LDVAR(NAME) _section$__DATA__ ## NAME
#define RESOURCE_LDLEN(NAME) (getsectbyname("__DATA", "__" #NAME)->size)
#elif (defined _WIN32)  /* mingw32 & mingw64 */
#ifdef __MINGW64__
#define RESOURCE_EXTLD(NAME) \
  extern const unsigned char _binary_ ## NAME ## _start[]; \
  extern const unsigned char _binary_ ## NAME ## _end[];
#define RESOURCE_LDVAR(NAME) \
  _binary_ ## NAME ## _start
#define RESOURCE_LDLEN(NAME) \
  ((_binary_ ## NAME ## _end) - (_binary_ ## NAME ## _start))
#else
#define RESOURCE_EXTLD(NAME) \
  extern const unsigned char binary_ ## NAME ## _start[]; \
  extern const unsigned char binary_ ## NAME ## _end[];
#define RESOURCE_LDVAR(NAME) \
  binary_ ## NAME ## _start
#define RESOURCE_LDLEN(NAME) \
  ((binary_ ## NAME ## _end) - (binary_ ## NAME ## _start))
#endif
#else /* gnu/linux ld */
#define RESOURCE_EXTLD(NAME) \
  extern const unsigned char _binary_ ## NAME ## _start[]; \
  extern const unsigned char _binary_ ## NAME ## _end[];
#define RESOURCE_LDVAR(NAME) \
  _binary_ ## NAME ## _start
#define RESOURCE_LDLEN(NAME) \
  ((_binary_ ## NAME ## _end) - (_binary_ ## NAME ## _start))
#endif


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Logger macros
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define LogWarning(...) \
    do { \
        std::string preString = std::string("WARNING in ") + std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) + std::string(" in ") + std::string(__func__) + std::string("(): "); \
        RD::Core::Console::MessageP(stdout, preString, __VA_ARGS__); \
    } while(0)
#define LogError(...) \
    do { \
        std::string preString = std::string("ERROR in ") + std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) + std::string(" in ") + std::string(__func__) + std::string("(): "); \
        RD::Core::Console::MessageP(stderr, preString, __VA_ARGS__); \
    } while(0)


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Type definitions, enums and structs
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/* Default module namespace */
namespace RD {
namespace Core {


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Prototypes
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**
 *  @brief Write binary data to a file.
 *  @param [in] in Bytes containing the binary data.
 *  @param [in] filePath The file name.
 *  @return True if success, false otherwise.
 */
bool WriteFile(const std::vector<uint8_t>& in, std::string filename);

/**
 *  @brief Write binary data to a file.
 *  @param [in] in Bytes containing the binary data.
 *  @param [in] filePath The file name.
 *  @return True if success, false otherwise.
 */
bool WriteFile(const std::vector<uint8_t>& in, const char* filename);

/**
 *  @brief Read binary data from file.
 *  @param [out] out Container where to store the read bytes.
 *  @param [in] filename The file name.
 *  @return True if success, false otherwise.
 */
bool ReadFile(std::vector<uint8_t>& out, std::string filename);

/**
 *  @brief Read binary data from file.
 *  @param [out] out Container where to store the read bytes.
 *  @param [in] filename The file name.
 *  @return True if success, false otherwise.
 */
bool ReadFile(std::vector<uint8_t>& out, const char* filename);

/**
 *  @brief Read a text file.
 *  @param [out] out String where to store the read bytes.
 *  @param [in] filename The file name.
 *  @return True if success, false otherwise.
 *  @details The file will be read in binary mode using the @ref ReadFile function and will then be converted to an std::string object.
 */
bool ReadFile(std::string& out, std::string filename);

/**
 *  @brief Read a text file.
 *  @param [out] out String where to store the read bytes.
 *  @param [in] filename The file name.
 *  @return True if success, false otherwise.
 *  @details The file will be read in binary mode using the @ref ReadFile function and will then be converted to an std::string object.
 */
bool ReadFile(std::string& out, const char* filename);

/**
 *  @brief Get the UTC time string.
 *  @return Time string, format: "YYYY-MM-DD HH:MM:SS".
 */
std::string GetTimeStringUTC(void);

/**
 *  @brief Get the local time string.
 *  @return Time string, format: "YYYY-MM-DD HH:MM:SS".
 */
std::string GetTimeStringLocal(void);

/**
 *  @brief Replace text in a string.
 *  @param [inout] text The text that should be modified.
 *  @param [in] replacement The replacement data.
 */
void ReplaceText(std::string& text, std::vector<std::pair<std::string, std::string>>& replacement);

/**
 *  @brief Get the absolute path of the application directory.
 *  @param [out] path The absolute path of the application without a separator at the end.
 *  @param [out] file Name of the application file including extension.
 *  @param [in] bufSize Size of buffer required to obtain path information, defaults to 1024.
 */
void GetAppDirectory(std::string& path, std::string& file, int bufSize = 1024);


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Classes
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**
 *  @brief Class: Console
 *  @details Draw text to the console (thread-safe).
 */
class Console {
    public:
        /**
         *  @brief Print a default message to the console.
         *  @param [in] stream The stream to which to print.
         *  @param [in] format Format string.
         *  @param [in] ... Variable arguments.
         */
        static void Message(FILE *stream, const char* format, ...);

        /**
         *  @brief Print a message to the console using a specific prefix string.
         *  @param [in] stream The stream to which to print.
         *  @param [in] preString The string that should be printed before the actual message.
         *  @param [in] format Format string.
         *  @param [in] ... Variable arguments.
         */
        static void MessageP(FILE *stream, std::string preString, const char* format, ...);

        /**
         *  @brief Print a hexdump.
         *  @param [in] buffer Buffer content.
         *  @param [in] length Buffer size.
         *  @param [in] stream The stream to which to print. Defaults to stdout.
         */
        static void HexDump(const uint8_t* buffer, size_t length, FILE* stream = stdout);

        /**
         *  @brief Print a hexdump.
         *  @param [in] buffer Buffer content.
         *  @param [in] stream The stream to which to print. Defaults to stdout.
         */
        static void HexDump(const std::vector<uint8_t>& buffer, FILE* stream = stdout);

    private:
        static std::mutex mtx; ///< Mutex to protect console prints.

}; /* class: Console */


} /* namespace: Core */
} /* namespace: RD */

