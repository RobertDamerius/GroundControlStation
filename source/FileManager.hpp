#pragma once


/* Directories */
#define DIRECTORY_SEPARATOR                    "/"
#define DIRECTORY_SHADER                       "gfx" DIRECTORY_SEPARATOR
#define DIRECTORY_TEXTURE                      "gfx" DIRECTORY_SEPARATOR
#define DIRECTORY_SKYBOX                       DIRECTORY_TEXTURE "skybox" DIRECTORY_SEPARATOR
#define DIRECTORY_CONFIGURATION                "configuration" DIRECTORY_SEPARATOR
#define DIRECTORY_VEHICLE                      "vehicle" DIRECTORY_SEPARATOR


/* File extensions */
#define FILE_EXTENSION_SHADER                  ".glsl"
#define FILE_EXTENSION_CONFIGURATION           ".cfg"
#define FILE_EXTENSION_PLY                     ".ply"


/* Skybox file names */
#define FILENAME_SKYBOX_X_POS                  "Right.png"
#define FILENAME_SKYBOX_X_NEG                  "Left.png"
#define FILENAME_SKYBOX_Y_POS                  "Top.png"
#define FILENAME_SKYBOX_Y_NEG                  "Bottom.png"
#define FILENAME_SKYBOX_Z_POS                  "Back.png"
#define FILENAME_SKYBOX_Z_NEG                  "Front.png"


/* Available files */
typedef enum {
    // Shaders
    FILE_SHADER_VEHICLE,
    FILE_SHADER_TRANSPARENT_VEHICLE,
    FILE_SHADER_GROUNDPLANE,
    FILE_SHADER_ENVIRONMENT,
    FILE_SHADER_POST_PROCESSING,
    FILE_SHADER_SCENE_TEXT,
    FILE_SHADER_LINE,
    FILE_SHADER_LINE_STRIP,
    FILE_SHADER_CIRCULAR_LINE_BUFFER,
    FILE_CONFIGURATION_NETWORK,
    FILE_CONFIGURATION_ORIGIN,
    FILE_CONFIGURATION_STYLE,
    FILE_CONFIGURATION_TTS,
    FILE_CONFIGURATION_DATABASE
} file_t;


/**
 *  @brief The FileManager manages file operations. It can be used to obtain the
 *  filename relative to the current directory from where the application has started.
 *  The @ref Initialize member function will extract the root directory from the
 *  first applications argument.
 */
class FileManager {
    public:
        /**
         *  @brief Initialize the file manager.
         *  @param [in] argv0 The first argument of the application.
         */
        static void Initialize(const char* argv0);

        /**
         *  @brief Get the filename including root directory.
         *  @param [in] file The file from which the name is requested.
         *  @return The filename.
         */
        static std::string GetName(file_t file);

        /**
         *  @brief Get the root directory.
         *  @return The root directory string.
         */
        static std::string GetRootDirectory(void);

    private:
        static std::string rootDirectory; ///< The applications root directory.
};

