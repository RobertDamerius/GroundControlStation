#include <FontSprite.hpp>


inline uint32_t next_power2(uint32_t n){
    n--;
    n |= n >> 16;
    n |= n >> 8;
    n |= n >> 4;
    n |= n >> 2;
    n |= n >> 1;
    return n + 1;
}


FontSprite::FontSprite(){
    this->texture = 0;
    Delete();
}

FontSprite::~FontSprite(){}

bool FontSprite::Generate(std::string filename, uint32_t fontsize, GLenum textureUnit, const GLenum textureFilter, const uint32_t spritePadding){
    std::vector<uint8_t> data;
    if(!RD::Core::ReadFile(data, filename) || !data.size()){
        LogError("Could not read file %s!\n",filename.c_str());
        return false;
    }
    return Generate((const uint8_t*)(&data[0]), (uint32_t)data.size(), fontsize, textureUnit, textureFilter, spritePadding);
}

bool FontSprite::Generate(const uint8_t* bytes, const uint32_t length, uint32_t fontsize, GLenum textureUnit, const GLenum textureFilter, const uint32_t spritePadding){
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Do not allow multiple loads, delete first
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Delete();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Init freetype and load font
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        FT_Library library;
        if(FT_Init_FreeType(&library)){
            LogError("Could not initialize freetype library!\n");
            return false;
        }
        FT_Face face;
        if(FT_New_Memory_Face(library, (FT_Byte*) &bytes[0], (FT_Long)length, (FT_Long)0, &face)){
            LogError("Could not create font face!\n");
            return false;
        }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Set font height in pixels
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if(FT_Set_Pixel_Sizes(face, 0, fontsize)){
            LogWarning("Could not set pixel size for font!\n");
        }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Structure to store all glyph sprites
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        struct s_bitmap {
            int32_t width;
            int32_t height;
            std::vector<uint8_t> data;
        };
        struct s_character {
            struct s_bitmap glyphBitmap;    // The raw loaded glyphBitmap
            int32_t glyphXOffset;            // Number of cols that must be added to the left before copying the glyphBitmap data
            int32_t glyphYOffset;           // Number of rows that must be added to the bottom before copying the glyphBitmap data
            int32_t glyphPxUnderBaseline;    // Number of pixels that are under the baseline pixel row
            struct s_bitmap spriteBitmap;    // The final spriteBitmap
        };
        struct s_character *characters = new struct s_character[256];

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Load bitmaps for all sprites
    // Get the maximum number of pixels over and under the baseline
    // The baseline is also a pixel row.
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        int maxOverBaseLine = 0;
        int maxUnderBaseLine = 0;
        for(int ch = 0; ch < 256 ; ch++){
            if(FT_Load_Char(face, ch, FT_LOAD_RENDER)){
                LogWarning("Could not load character %d!\n",ch);
                continue;
            }
            if((int)(face->glyph->metrics.width >> 6) != (int)face->glyph->bitmap.width){
                LogWarning("Could not load character %d. Different width!\n",ch);
                continue;
            }
            if((int)(face->glyph->metrics.height >> 6) != (int)face->glyph->bitmap.rows){
                printf("Could not load character %d. Different height!",ch);
                continue;
            }
            // Get pixels rows over and under the baseline
            int overbaseline = (int32_t)(face->glyph->metrics.horiBearingY >> 6);
            int underbaseline = ((int32_t)(face->glyph->metrics.height >> 6) - 1 - (int32_t)(face->glyph->metrics.horiBearingY >> 6));
            //? TODO: What to do with negative x,y offsets?
            // overbaseline = (overbaseline < 0) ? 0 : overbaseline;
            // underbaseline = (underbaseline < 0) ? 0 : underbaseline;
            maxOverBaseLine = overbaseline > maxOverBaseLine ? overbaseline : maxOverBaseLine;
            maxUnderBaseLine = underbaseline > maxUnderBaseLine ? underbaseline : maxUnderBaseLine;
            // Copy to character list
            characters[ch].spriteBitmap.width = (int32_t)(face->glyph->metrics.horiAdvance >> 6); // Save width for the current sprite bitmap (the height is calculated after this loop)
            characters[ch].glyphXOffset = (int32_t)(face->glyph->metrics.horiBearingX >> 6);
            if(characters[ch].glyphXOffset < 0){
                characters[ch].spriteBitmap.width -= characters[ch].glyphXOffset;
                characters[ch].glyphXOffset = 0;
            }
            characters[ch].glyphPxUnderBaseline = underbaseline;
            characters[ch].glyphBitmap.width = (int32_t)(face->glyph->metrics.width >> 6);
            characters[ch].glyphBitmap.height = (int32_t)(face->glyph->metrics.height >> 6);
            // Copy bytes in correct order: we want to begin from left bottom corner, not from left top corner
            for(int y = (int)face->glyph->bitmap.rows-1; y >= 0; y--){
                for(int x = 0; x < (int)face->glyph->bitmap.width; x++){
                    characters[ch].glyphBitmap.data.push_back((uint8_t)face->glyph->bitmap.buffer[x + y*(int)face->glyph->bitmap.width]);
                }
            }
        }
        this->baseline = maxUnderBaseLine;
        fontHeight = maxOverBaseLine + 1 + maxUnderBaseLine; // Final size is overbaseline + baseline (1) + underbaseline
        // Calculate the yOffset for all glyphs, also set the height for the final sprite bitmap
        for(int ch = 0; ch < 256; ch++){
            characters[ch].glyphYOffset = maxUnderBaseLine - characters[ch].glyphPxUnderBaseline;
            characters[ch].spriteBitmap.height = fontHeight;
        }
        // Now create all sprite bitmaps with correct margin and also save informations about the final sprite image dimension
        int maxHeight = fontHeight * 16;
        int maxWidth = 0, currentWidth = 0;
        for(int ch = 0; ch < 256; ch++){
            if(!(ch % 16))
                currentWidth = 0;
            std::vector<uint8_t> tmp(characters[ch].spriteBitmap.width * characters[ch].spriteBitmap.height, 0x00);
            for(int y = 0; y < characters[ch].glyphBitmap.height; y++){
                for(int x = 0; x < characters[ch].glyphBitmap.width; x++){
                    int index = (characters[ch].glyphXOffset + x) + (characters[ch].glyphYOffset + y) * characters[ch].spriteBitmap.width;
                    if((index >= 0) && (index < (int)tmp.size()))
                        tmp[index] = characters[ch].glyphBitmap.data[x + y * characters[ch].glyphBitmap.width];
                }
            }
            characters[ch].spriteBitmap.data.swap(tmp);
            currentWidth += characters[ch].spriteBitmap.width;
            maxWidth = currentWidth > maxWidth ? currentWidth : maxWidth;
        }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Let's merge all sprite bitmaps to a big sprite image
    // The size should be power-of-two so we use next size.
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        textureWidth = next_power2((maxWidth + 17*spritePadding));
        textureHeight = next_power2((maxHeight + 17*spritePadding));
        std::vector<uint8_t> spriteImageData(textureWidth * textureHeight, 0x00);
        for(int ch = 0, x_offset = 0, y_offset = -fontHeight; ch < 256; ch++){
            if(!(ch % 16)){
                x_offset = spritePadding;
                y_offset += fontHeight + spritePadding;
            }
            for(int y = 0; y < characters[ch].spriteBitmap.height; y++){
                for(int x = 0; x < characters[ch].spriteBitmap.width; x++){
                    int index = (x + x_offset) + (y + y_offset) * textureWidth;
                    spriteImageData[index] = characters[ch].spriteBitmap.data[x + y * characters[ch].spriteBitmap.width];
                }
            }
            // Left Bottom Corner
            textureCoordinateTable[ch].x = (GLfloat)((double)x_offset / (double)textureWidth);
            textureCoordinateTable[ch].y = (GLfloat)((double)y_offset / (double)textureHeight);
            // Increment x_offset to right edge sprite
            x_offset += (spriteWidthTable[ch] = characters[ch].spriteBitmap.width);
            // Right Top Corner
            textureCoordinateTable[ch].z = (GLfloat)((double)x_offset / (double)textureWidth);
            textureCoordinateTable[ch].w = (GLfloat)((double)(y_offset + fontHeight) / (double)textureHeight);
            // Increment x_offset to left neighbor sprite
            x_offset += spritePadding;
        }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Generate OpenGL texture
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        DEBUG_GLCHECK( glActiveTexture(textureUnit); );
        DEBUG_GLCHECK( glGenTextures(1, &texture); );
        DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, texture); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilter); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilter); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); );
        DEBUG_GLCHECK( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)textureWidth, (GLsizei)textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, (GLubyte*)&spriteImageData[0]); );
        DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, 0); );
        DEBUG_GLCHECK( glActiveTexture(GL_TEXTURE0); );

        FT_Done_Face(face);
        FT_Done_FreeType(library);
        delete[] characters;
        return true;
}

void FontSprite::Delete(void){
    if(texture){
        DEBUG_GLCHECK( glDeleteTextures(1, &texture); );
        texture = 0;
    }
    fontHeight = 0;
    baseline = 0;
    textureWidth = 0;
    textureHeight = 0;
    for(int i = 0; i < 256; i++){
        textureCoordinateTable[i] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        spriteWidthTable[i] = 0;
    }
}

GLuint FontSprite::GetTexture(void){
    return this->texture;
}

uint32_t FontSprite::GetTextureWidth(void){
    return this->textureWidth;
}

uint32_t FontSprite::GetTextureHeight(void){
    return this->textureHeight;
}

double FontSprite::GetLineHeight(void){
    return this->fontHeight;
}

double FontSprite::GetBaseline(void){
    return this->baseline;
}

void FontSprite::TextToVertices(std::vector<font_sprite_vertex_t>& outVertices, glm::vec2& outDimension, const std::string& inText, const uint8_t tabSize){
    // String preprocessing.
    // Only use extended-ascii characters in range 32 ... 255 and the <LF> ('\n') character.
    // Replace TABs by SPACEs.
    std::vector<uint8_t> characters;
    for(size_t n = 0; n < inText.size(); n++){
        uint32_t u = (uint32_t)inText[n];
        if(u < 256){
            if((uint32_t)('\t') == u){
                for(uint8_t k = 0; k < tabSize; k++)
                    characters.push_back(0x20);
                continue;
            }
            if((u > 31) || (uint32_t('\n') == u)){
                characters.push_back((uint8_t)u);
            }
        }
    }

    // Create new vertices.
    // The first character starts at [0.0, -baseline].
    outVertices.clear();
    GLfloat currentWidth = 0.0f;
    GLfloat currentHeight = -(GLfloat)baseline;
    GLfloat sHeight = (GLfloat)fontHeight;
    outDimension.x = 0.0f;
    outDimension.y = sHeight;
    for(size_t n = 0; n < characters.size(); n++){
        if(uint8_t('\n') == characters[n]){
            if(currentWidth > outDimension.x)
                outDimension.x = currentWidth;
            outDimension.y += sHeight;
            currentWidth = 0.0f;
            currentHeight -= sHeight;
            continue;
        }
        font_sprite_vertex_t v[4];
        GLfloat sWidth = (GLfloat)spriteWidthTable[characters[n]];
        glm::vec4 tCoord = textureCoordinateTable[characters[n]]; // s1, t1, s2, t2

        // Top Left
        v[0].x = currentWidth;
        v[0].y = currentHeight + sHeight;
        v[0].s = tCoord.x;
        v[0].t = tCoord.w;

        // Bottom Left
        v[1].x = currentWidth;
        v[1].y = currentHeight;
        v[1].s = tCoord.x;
        v[1].t = tCoord.y;

        // Bottom Right
        v[2].x = currentWidth + sWidth;
        v[2].y = currentHeight;
        v[2].s = tCoord.z;
        v[2].t = tCoord.y;

        // Top Right
        v[3].x = currentWidth + sWidth;
        v[3].y = currentHeight + sHeight;
        v[3].s = tCoord.z;
        v[3].t = tCoord.w;

        for(int i = 0; i < 4; i++)
            outVertices.push_back(v[i]);
        if((currentWidth += sWidth) > outDimension.x)
            outDimension.x = currentWidth;
    }
    outVertices.shrink_to_fit();
}

