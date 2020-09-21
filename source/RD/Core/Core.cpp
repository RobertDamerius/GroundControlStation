/**
 *  @file Core.cpp
 *  @details Version 20200513.
 */
#include <Core.hpp>


bool RD::Core::WriteFile(const std::vector<uint8_t>& in, std::string filename){
    return RD::Core::WriteFile(in, filename.c_str());
}

bool RD::Core::WriteFile(const std::vector<uint8_t>& in, const char* filename){
    // Open file
        FILE *fp = fopen(filename, "wb");
        if(!fp)
            return false;

    // Write bytes to file
        size_t bytesWritten = fwrite((const void*)&in[0], sizeof(uint8_t), in.size(), fp);
        fclose(fp);
        return (in.size() == bytesWritten);
}

bool RD::Core::ReadFile(std::vector<uint8_t>& out, std::string filename){
    return RD::Core::ReadFile(out, filename.c_str());
}

bool RD::Core::ReadFile(std::vector<uint8_t>& out, const char* filename){
    // Clear reference
        out.clear();

    // Open file
        FILE *fp = fopen(filename, "rb");
        if(!fp)
            return false;

    // Obtain file size
        fseek(fp, 0, SEEK_END);
        long int fpsize = ftell(fp);
        if(fpsize < 0){
            fclose(fp);
            return false;
        }

    // Return success if file contains no bytes
        fseek(fp, 0, SEEK_SET);
        if(!fpsize){
            fclose(fp);
            return true;
        }

    // Read bytes from file
        std::vector<uint8_t> buf(fpsize);
        size_t bytesRead = fread((void*)&buf[0], sizeof(uint8_t), buf.size(), fp);
        fclose(fp);
        out.swap(buf);
        return (bytesRead == (size_t)fpsize);
}

bool RD::Core::ReadFile(std::string& out, std::string filename){
    return RD::Core::ReadFile(out, filename.c_str());
}

bool RD::Core::ReadFile(std::string& out, const char* filename){
    // Clear reference
        out.clear();

    // Read bytes from file
        std::vector<uint8_t> buf;
        if(!ReadFile(buf, filename))
            return false;

    // Copy to reference
        out = std::string(buf.begin(), buf.end());
        return true;
}

std::string RD::Core::GetTimeStringUTC(void){
    time_t t;
    time(&t);
    struct tm *timeInfo = gmtime(&t);
    char buffer[64];
    if(!timeInfo)
        goto error;
    if(!strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", timeInfo))
        goto error;
    return std::string(buffer);
    error:
    return std::string("0000-00-00 00:00:00");
}

std::string RD::Core::GetTimeStringLocal(void){
    time_t t;
    time(&t);
    struct tm *timeInfo = localtime(&t);
    char buffer[64];
    if(!timeInfo)
        goto error;
    if(!strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", timeInfo))
        goto error;
    return std::string(buffer);
    error:
    return std::string("0000-00-00 00:00:00");
}

void RD::Core::ReplaceText(std::string& text, std::vector<std::pair<std::string, std::string>>& replacement){
    for (std::pair<std::string, std::string> fromToPair: replacement) {
        size_t start_pos = 0;
        while((start_pos = text.find(fromToPair.first, start_pos)) != std::string::npos){
            text.replace(start_pos, fromToPair.first.length(), fromToPair.second);
            start_pos += fromToPair.second.length();
        }
    }
}

void RD::Core::GetAppDirectory(std::string& path, std::string& file, int bufSize){
    char* buffer = new char[bufSize];
    #ifdef __WIN32__
    DWORD len = GetModuleFileName(NULL, (LPSTR)(&buffer[0]), (DWORD)bufSize);
    #else
    ssize_t len = readlink("/proc/self/exe", &buffer[0], bufSize);
    #endif
    std::string str(buffer, len);
    auto found = str.find_last_of("/\\");
    path = str.substr(0, found);
    file = str.substr(found + 1);
    delete[] buffer;
}

