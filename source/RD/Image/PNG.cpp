/**
 *  @file PNG.cpp
 *  @details Version 20210203.
 */
#include <Image.hpp>
#include <lodepng.h>
#include <Core.hpp>


RD::Image::PNG::PNG(){
    width = 0;
    height = 0;
    numChannels = 0;
}

bool RD::Image::PNG::Read(std::string filename, uint32_t numChannels, bool mirror){
    // Set default fallback values
    this->width = 0;
    this->height = 0;
    this->numChannels = 0;
    this->data.clear();

    // Read file
    std::vector<uint8_t> bytes;
    if(!RD::Core::ReadFile(bytes, filename)){
        return false;
    }
    return this->Read(bytes, numChannels, mirror);
}

bool RD::Image::PNG::Read(std::vector<uint8_t>& bytes, uint32_t numChannels, bool mirror){
    // Set default fallback values
        this->width = 0;
        this->height = 0;
        this->numChannels = 0;
        this->data.clear();

    // Check for correct input size and valid number of channels
        if(!bytes.size() || ((3 != numChannels) && (4 != numChannels))){
            return false;
        }

    // Decode png image (RGBA with 8 bits per channel)
        std::vector<unsigned char> buf;
        unsigned w, h;
        unsigned error = lodepng::decode(buf, w, h, bytes, LCT_RGBA, 8);
        if(error){
            return false;
        }

    // Obtain decoded image size and desired image size (depending on numChannels)
        uint64_t uiSize = (uint64_t)buf.size();
        uint64_t dataSize = (uint64_t)w * (uint64_t)h * (uint64_t)numChannels;

    // Swap rows if parameter mirror is true, otherwise simply copy to output
        if(mirror){
            uint64_t n, dn = 0;
            for(uint64_t d = 0, i = uiSize - ((uint64_t)w << 2); d < dataSize; d += numChannels, i += 4){
                n = (uint64_t)(d / ((uint64_t)w * (uint64_t)numChannels));
                if((n - dn)){
                    i = (uint64_t)(uiSize - (1 + n) * ((uint64_t)w << 2));
                    dn = n;
                }
                data.push_back(buf[i]);
                data.push_back(buf[i + 1]);
                data.push_back(buf[i + 2]);
                if(4 == numChannels)
                    data.push_back(buf[i + 3]);
            }
        }
        else{
            for(uint64_t n = 0, i = 0; n < dataSize; n += (uint64_t)numChannels, i += 4){
                data.push_back(buf[i]);
                data.push_back(buf[i + 1]);
                data.push_back(buf[i + 2]);
                if(4 == numChannels)
                    data.push_back(buf[i + 3]);
            }
        }

    // Success
    this->width = (uint32_t)w;
    this->height = (uint32_t)h;
    this->numChannels = numChannels;
    return true;
}

bool RD::Image::PNG::Write(std::string filename, bool mirror){
    std::vector<uint8_t> bytes;
    if(!this->Write(bytes, mirror))
        return false;
    return RD::Core::WriteFile(bytes, filename);
}

bool RD::Image::PNG::Write(std::vector<uint8_t>& bytes, bool mirror){
    // Clear output
    bytes.clear();

    // Check for correct inputs
    if((data.size() != ((uint64_t)width * (uint64_t)height * (uint64_t)numChannels)) || ((3 != numChannels) && (4 != numChannels)))
        return false;

    // Convert input to RGBA image
    std::vector<uint8_t> img((width * height) * 4);
    for(uint32_t y = 0; y < height; y++){
        uint32_t yw = y * width;
        uint32_t ywInput = (mirror ? (height - 1 - y) : (y)) * width;
        for(uint32_t x = 0; x < width; x++){
            uint32_t idx = (x + yw) << 2;
            uint32_t idxInput = (x + ywInput)*numChannels;
            img[idx] = data[idxInput];
            img[idx+1] = data[idxInput+1];
            img[idx+2] = data[idxInput+2];
            img[idx+3] = (4 == numChannels) ? data[idxInput+3] : 0xFF;
        }
    }

    // Encode image and return success
    unsigned error = lodepng::encode(bytes, img, (unsigned)width, (unsigned)height, LCT_RGBA);
    return !error;
}

