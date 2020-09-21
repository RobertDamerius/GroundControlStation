#include <Skybox.hpp>
#include <FileManager.hpp>
#include <Common.hpp>


Skybox::Skybox(){
    dataAvailable = false;
    texture = 0;
    vao = 0;
    vbo = 0;
}

Skybox::~Skybox(){}

bool Skybox::ReadTextures(void){
    // Clear existing data
    for(int i = 0; i < 6; i++)
        faces[i].data.clear();
    dataAvailable = false;

    // Obtain filenames for all six faces
    std::string directory = FileManager::GetRootDirectory() + std::string(DIRECTORY_SKYBOX);
    std::string fileNames[6];
    fileNames[0] = directory + std::string(FILENAME_SKYBOX_X_POS);
    fileNames[1] = directory + std::string(FILENAME_SKYBOX_X_NEG);
    fileNames[2] = directory + std::string(FILENAME_SKYBOX_Y_POS);
    fileNames[3] = directory + std::string(FILENAME_SKYBOX_Y_NEG);
    fileNames[4] = directory + std::string(FILENAME_SKYBOX_Z_POS);
    fileNames[5] = directory + std::string(FILENAME_SKYBOX_Z_NEG);

    // Read all six faces
    for(int i = 0; i < 6; i++){
        if(!faces[i].Read(fileNames[i], 3)){
            for(int k = 0; k <= i; k++){
                faces[k].width = 0;
                faces[k].height = 0;
                faces[k].data.clear();
            }
            LogError("Could not read file \"%s\"!\n",fileNames[i].c_str());
            return false;
        }
    }

    // Return success
    return (dataAvailable = true);
}

bool Skybox::GenerateGL(bool clearData){
    // Make sure that the skybox is deleted
    DeleteGL();

    // If no data available: error
    if(!dataAvailable){
        LogWarning("GenerateGL() called without ReadTextures() being called first!\n");
        return false;
    }

    // Bind texture and generate cubemap
    DEBUG_GLCHECK( glActiveTexture(TEXTUREUNIT_SKYBOX); );
    DEBUG_GLCHECK( glGenTextures(1, &texture); );
    DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_CUBE_MAP, texture); );
    DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); );
    DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); );
    DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); );
    DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); );
    DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); );
    for(int i = 0; i < 6; i++){
        DEBUG_GLCHECK( glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, faces[i].width, faces[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, &(faces[i].data[0])); );
        if(clearData){
            faces[i].data.clear();
        }
    }

    // Create VAO for cube
    const GLfloat skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    DEBUG_GLCHECK( glGenVertexArrays(1, &vao); );
    DEBUG_GLCHECK( glGenBuffers(1, &vbo); );
    DEBUG_GLCHECK( glBindVertexArray(vao); );
        DEBUG_GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vbo); );
        DEBUG_GLCHECK( glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices[0], GL_STATIC_DRAW); );
        DEBUG_GLCHECK( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(0); );
    DEBUG_GLCHECK( glBindVertexArray(0); );

    // Return success
    return true;
}

void Skybox::DeleteGL(void){
    if(texture){
        glDeleteTextures(1, &texture);
        texture = 0;
    }
    if(vbo){
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    if(vao){
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}

void Skybox::DrawCube(void){
    DEBUG_GLCHECK( glActiveTexture(TEXTUREUNIT_SKYBOX); );
    DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_CUBE_MAP, texture); );
    DEBUG_GLCHECK( glBindVertexArray(vao); );
    DEBUG_GLCHECK( glDrawArrays(GL_TRIANGLES, 0, 36); );
}

