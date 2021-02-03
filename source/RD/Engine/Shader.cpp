/**
 *  @file Shader.cpp
 *  @details Version 20210203.
 */
#include <Engine.hpp>


std::string RD::Engine::Shader::GetShadingLanguageVersion(void){
    // Get string that looks like "4.40 - Build 21.20.16.4727"
    std::string strGLSL((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Only use substring to first " "
    std::size_t pos = strGLSL.find_first_of(" ");
    if(std::string::npos != pos){
        strGLSL = strGLSL.substr(0, pos);
    }

    // Only use numbers
    std::string result;
    for(auto c : strGLSL){
        if((c >= '0') && (c <= '9'))
            result.push_back(c);
    }
    return result;
}

bool RD::Engine::Shader::Generate(std::string fileName){
    return Generate(fileName, GetShadingLanguageVersion());
}

bool RD::Engine::Shader::Generate(std::string fileName, std::string version){
    std::vector<uint8_t> fileData;
    if(!RD::Core::ReadFile(fileData, fileName))
        return false;
    return Generate(fileData, version);
}

bool RD::Engine::Shader::Generate(std::string fileName, std::string version, std::vector<std::pair<std::string, std::string>>& replacement){
    std::vector<uint8_t> fileData;
    if(!RD::Core::ReadFile(fileData, fileName))
        return false;
    return Generate(fileData, version, replacement);
}

bool RD::Engine::Shader::Generate(std::vector<uint8_t>& fileData, std::string version){
    std::vector<std::pair<std::string, std::string>> emptyReplacement;
    return Generate(fileData, version, emptyReplacement);
}

bool RD::Engine::Shader::Generate(std::vector<uint8_t>& fileData, std::string version, std::vector<std::pair<std::string, std::string>>& replacement){
    GLuint idShaderVS, idShaderGS = 0, idShaderFS;
    GLint success, len;
    GLchar *src;
    std::string strSource;

    // Replace text and add zero terminator
    std::string shaderSource(fileData.begin(), fileData.end());
    RD::Core::ReplaceText(shaderSource, replacement);
    shaderSource.push_back(0x00);

    // Prefix management
    std::string prefixVersion = std::string("#version ") + version + std::string("\n");
    std::string prefixVS = prefixVersion + std::string("#define VERTEX_SHADER\n");
    std::string prefixGS = prefixVersion + std::string("#define GEOMETRY_SHADER\n");
    std::string prefixFS = prefixVersion + std::string("#define FRAGMENT_SHADER\n");

    // Create vertex shader
    strSource = prefixVS + shaderSource;
    idShaderVS = glCreateShader(GL_VERTEX_SHADER);
    src = (GLchar*)strSource.c_str();
    len = (GLint)strSource.length();
    DEBUG_GLCHECK( glShaderSource(idShaderVS, 1, &src, &len); );
    DEBUG_GLCHECK( glCompileShader(idShaderVS); );
    DEBUG_GLCHECK( glGetShaderiv(idShaderVS, GL_COMPILE_STATUS, &success); );
    if(!success){
        GLint logSize = 0;
        DEBUG_GLCHECK( glGetShaderiv(idShaderVS, GL_INFO_LOG_LENGTH, &logSize); );
        if(logSize){
            GLchar *infoLog = new GLchar[logSize];
            DEBUG_GLCHECK( glGetShaderInfoLog(idShaderVS, logSize, nullptr, infoLog); );
            LogError("\nVERTEX SHADER ERROR:\n%s\n", (const char*)infoLog);
            delete[] infoLog;
        }
        else{
            LogError("Could not get log size for vertex shader information!\n");
        }
        return false;
    }

    // Create geometry shader
    bool useGeometryShader = (std::string::npos != shaderSource.find("GEOMETRY_SHADER"));
    if(useGeometryShader){
        strSource = prefixGS + shaderSource;
        idShaderGS = glCreateShader(GL_GEOMETRY_SHADER);
        src = (GLchar*)strSource.c_str();
        len = (GLint)strSource.length();
        DEBUG_GLCHECK( glShaderSource(idShaderGS, 1, &src, &len); );
        DEBUG_GLCHECK( glCompileShader(idShaderGS); );
        DEBUG_GLCHECK( glGetShaderiv(idShaderGS, GL_COMPILE_STATUS, &success); );
        if(!success){
            GLint logSize = 0;
            DEBUG_GLCHECK( glGetShaderiv(idShaderGS, GL_INFO_LOG_LENGTH, &logSize); );
            if(logSize){
                GLchar *infoLog = new GLchar[logSize];
                DEBUG_GLCHECK( glGetShaderInfoLog(idShaderGS, logSize, nullptr, infoLog); );
                LogError("\nGEOMETRY SHADER ERROR:\n%s\n", (const char*)infoLog);
                delete[] infoLog;
            }
            else{
                LogError("Could not get log size for geometry shader information!\n");
            }
            return false;
        }
    }

    // Create fragment shader
    strSource = prefixFS + shaderSource;
    idShaderFS = glCreateShader(GL_FRAGMENT_SHADER);
    src = (GLchar*)strSource.c_str();
    len = (GLint)strSource.length();
    DEBUG_GLCHECK( glShaderSource(idShaderFS, 1, &src, &len); );
    DEBUG_GLCHECK( glCompileShader(idShaderFS); );
    DEBUG_GLCHECK( glGetShaderiv(idShaderFS, GL_COMPILE_STATUS, &success); );
    if(!success){
        GLint logSize = 0;
        DEBUG_GLCHECK( glGetShaderiv(idShaderFS, GL_INFO_LOG_LENGTH, &logSize); );
        if(logSize){
            GLchar *infoLog = new GLchar[logSize];
            DEBUG_GLCHECK( glGetShaderInfoLog(idShaderFS, logSize, nullptr, infoLog); );
            LogError("\nFRAGMENT SHADER ERROR:\n%s\n", (const char*)infoLog);
            delete[] infoLog;
        }
        else{
            LogError("Could not get log size for fragment shader information!\n");
        }
        glDeleteShader(idShaderVS);
        return false;
    }

    // Create shader program
    this->id = glCreateProgram();
    DEBUG_GLCHECK( glAttachShader(this->id, idShaderVS); );
    if(useGeometryShader){
        DEBUG_GLCHECK( glAttachShader(this->id, idShaderGS); );
    }
    DEBUG_GLCHECK( glAttachShader(this->id, idShaderFS); );
    DEBUG_GLCHECK( glLinkProgram(this->id); );
    DEBUG_GLCHECK( glGetProgramiv(this->id, GL_LINK_STATUS, &success); );
    if(!success){
        GLint logSize = 0;
        DEBUG_GLCHECK( glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &logSize); );
        if(logSize){
            GLchar *infoLog = new GLchar[logSize];
            DEBUG_GLCHECK( glGetShaderInfoLog(this->id, logSize, nullptr, infoLog); );
            LogError("\nSHADER LINK ERROR:\n%s\n", (const char*)infoLog);
            delete[] infoLog;
        }
        else{
            LogError("Could not get log size for shader program information!\n");
        }
        glDeleteShader(idShaderVS);
        if(useGeometryShader){
            glDeleteShader(idShaderGS);
        }
        glDeleteShader(idShaderFS);
        return false;
    }

    // Flag attached shaders for deletion
    glDeleteShader(idShaderVS);
    if(useGeometryShader){
        glDeleteShader(idShaderGS);
    }
    glDeleteShader(idShaderFS);

    // Success
    return true;
}

void RD::Engine::Shader::Delete(void){
    if(id){
        DEBUG_GLCHECK( glDeleteProgram(id); );
        id = 0;
    }
}

