/**
 *  @file Console.cpp
 *  @details Version 20200513.
 */
#include <Core.hpp>


std::mutex RD::Core::Console::mtx;


void RD::Core::Console::Message(FILE *stream, const char* format, ...){
    const std::lock_guard<std::mutex> lock(RD::Core::Console::mtx);
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stream, format, argptr);
    va_end(argptr);
    fflush(stream);
}

void RD::Core::Console::MessageP(FILE *stream, std::string preString, const char* format, ...){
    const std::lock_guard<std::mutex> lock(RD::Core::Console::mtx);
    va_list argptr;
    va_start(argptr, format);
    if(preString.size()) fprintf(stream, "%s", preString.c_str());
    if(argptr) vfprintf(stream, format, argptr);
    else fprintf(stream, "%s", format);
    fflush(stream);
    va_end(argptr);
}

void RD::Core::Console::HexDump(const uint8_t* buffer, size_t length, FILE* stream){
    const std::lock_guard<std::mutex> lock(RD::Core::Console::mtx);
    fprintf(stream, "Offset(h)  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
    for(size_t n = 0; n < length; n++){
        if(!(n % 16)){
            if(n){
                fprintf(stream, "  ");
                std::string plainText;
                for(int i = 0; i < 16; i++){
                    unsigned char c = buffer[n - 16 + i];
                    plainText.push_back((c < 0x20) ? '.' : c);
                }
                fprintf(stream, "%s", plainText.c_str());
            }
            fprintf(stream, "\n %08X ", (uint32_t)n);
        }
        fprintf(stream," %02X", buffer[n]);
        if(n == (length - 1)){
            std::string wspace = "  ";
            for(int i = (int)(n % 16); i < 15; i++)
                wspace += "   ";
            fprintf(stream, "%s", wspace.c_str());
            std::string plainText;
            for(size_t k = length - (n % 16) - 1; k < length; k++)
                plainText.push_back((buffer[k] < 0x20) ? '.' : buffer[k]);
            fprintf(stream, "%s", plainText.c_str());
        }
    }
    fprintf(stream, "\n");
    fflush(stream);
}

void RD::Core::Console::HexDump(const std::vector<uint8_t>& buffer, FILE* stream){
    RD::Core::Console::HexDump(&buffer[0], buffer.size(), stream);
}

