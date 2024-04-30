/**
 *  @file Engine.hpp
 *  @brief The engine header.
 *  @details Version 20210203.
 *  The OpenGL headers should be included in the following way:
 *     #define GLEW_STATIC
 *     #include <GL/glew.h>
 *     #ifdef _WIN32
 *     #define GLFW_EXPOSE_NATIVE_WGL
 *     #define GLFW_EXPOSE_NATIVE_WIN32
 *     #endif
 *     #include <GLFW/glfw3.h>
 *     #include <GLFW/glfw3native.h>
 *     #include <glm/glm.hpp>
 *     #include <glm/gtc/matrix_transform.hpp>
 *     #include <glm/gtc/type_ptr.hpp>
 */
#pragma once


#include <Core.hpp>


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Debug macros for GL
// Example: DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, 0); );
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef DEBUG
#define DEBUG_GLCHECK(stmt)    \
    do { \
        GLenum e = glGetError(); \
        stmt; \
        if(GL_NO_ERROR != (e = glGetError())) \
            RD::Core::Console::Message(stderr,"%s:%d in %s() \"%s\": %s\n", __FILE__, __LINE__, __func__, #stmt, RD::Engine::GLErrorToString(e).c_str()); \
    } while(0)
#else
#define DEBUG_GLCHECK(stmt) stmt
#endif /* DEBUG */


/* Default module namespace */
namespace RD {
namespace Engine {


/**
 *  @brief Convert GL error to corresponding string.
 *  @param [in] error GL error enum.
 *  @return String that names the GL error.
 */
std::string GLErrorToString(GLenum error);

/**
 *  @brief Get OpenGL information.
 *  @param [out] versionGL GL version string.
 *  @param [out] versionGLSL GLSL version string.
 *  @param [out] vendor Vendor string.
 *  @param [out] renderer Renderer string.
 */
void GetGLInfo(std::string* versionGL, std::string* versionGLSL, std::string* vendor, std::string* renderer);


/* Camera Modes */
typedef enum{
    CAMERA_MODE_PERSPECTIVE,
    CAMERA_MODE_ORTHOGRAPHIC
} CameraMode;


/**
 *  @brief Class: Camera
 */
class Camera {
    public:
        CameraMode mode;       ///< The camera mode. Either @ref CAMERA_MODE_ORTHOGRAPHIC or @ref CAMERA_MODE_PERSPECTIVE.
        double left;           ///< The left border limit for orthographic projection.
        double right;          ///< The right border limit for orthographic projection.
        double bottom;         ///< The bottom border limit for orthographic projection.
        double top;            ///< The top border limit for orthographic projection.
        double clipNear;       ///< The near clipping pane for perspective projection.
        double clipFar;        ///< The far clipping pane for perspective projection.
        double aspect;         ///< The aspect ratio for perspective projection.
        double fov;            ///< The field of view angle in radians for perspective projection.
        glm::dvec3 position;   ///< The position in world space coordinates.
        glm::dvec3 view;       ///< The view direction in world space coordinates.
        glm::dvec3 up;         ///< The up direction in world space coordinates.

        /**
         *  @brief Update the projection matrix.
         *  @details Call this function if you changed the projection mode or projection parameters (left, right, bottom, top, fov, aspect, clipNear, clipFar).
         */
        void UpdateProjectionMatrix(void);

        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // GENERAL
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief Create a camera using a perspective projection. This is the default constructor.
         *  @param [in] clipNear Near clipping pane border. Defaults to 0.01.
         *  @param [in] clipFar Far clipping pane border. Defaults to 500.0.
         *  @param [in] aspect The aspect ratio (width / height). Defaults to 8.0 / 5.0.
         *  @param [in] fov The field of view in radians. Defaults to glm::radians(58.0).
         *  @param [in] position The initial position. Defaults to glm::dvec3(0.0).
         *  @param [in] view The initial view direction. Defaults to glm::dvec3(0.0, 0.0, -1.0).
         *  @param [in] up The initial up direction. Defaults to glm::dvec3(0.0, 1.0, 0.0).
         *  @details The private attribute @ref mode will be set to @ref CAMERA_MODE_PERSPECTIVE.
         */
        Camera(double clipNear = 0.01, double clipFar = 500.0, double aspect = 8.0 / 5.0, double fov = glm::radians(70.0), glm::dvec3 position = glm::dvec3(0.0), glm::dvec3 view = glm::dvec3(0.0, 0.0, -1.0), glm::dvec3 up = glm::dvec3(0.0, 1.0, 0.0));

        /**
         *  @brief Create a camera using an orthographic projection.
         *  @param [in] left Left border.
         *  @param [in] right Right border.
         *  @param [in] bottom Bottom border.
         *  @param [in] top Top border.
         *  @param [in] clipNear Near clipping pane border.
         *  @param [in] clipFar Far clipping pane border.
         *  @param [in] view The view direction vector. Defaults to glm::dvec3(0.0, 0.0, -1.0).
         *  @param [in] up The up direction vector. Defaults to glm::dvec3(0.0, 1.0, 0.0).
         *  @details The private attribute @ref mode will be set to @ref CAMERA_MODE_ORTHOGRAPHIC.
         */
        Camera(double left, double right, double bottom, double top, double clipNear, double clipFar, glm::dvec3 view = glm::dvec3(0.0, 0.0, -1.0), glm::dvec3 up = glm::dvec3(0.0, 1.0, 0.0));

        /**
         *  @brief Default copy constructor.
         */
        Camera(const Camera& camera) = default;

        /**
         *  @brief Delete the camera.
         *  @details @ref DeleteUniformBufferObject will NOT be called.
         */
        ~Camera();

        /**
         *  @brief Assignment operator.
         *  @param [in] rhs The right hand side value.
         *  @details This will copy all private attributes from rhs to this camera instance. However, the UBO value will not be copied.
         */
        Camera& operator=(const Camera& rhs);


        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // WORLD SPACE TRANSFORMATIONS
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief Rotate camera in world space coordinates beginning from the current orientation.
         *  @param [in] angle Angle in radians.
         *  @param [in] axis Rotation axis.
         */
        void Rotate(double angle, glm::dvec3 axis);


        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // VIEW SPACE TRANSFORMATIONS
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief Roll around the forward (view) direction.
         *  @param [in] angle Roll angle in radians.
         */
        void RollView(double angle);

        /**
         *  @brief Pitch around the x-axis of the camera view frame.
         *  @param [in] angle Pitch angle in radians.
         */
        void PitchView(double angle);

        /**
         *  @brief Yaw around the down direction of the camera view frame.
         *  @param [in] angle Yaw angle in radians.
         */
        void YawView(double angle);

        /**
         *  @brief Move camera in the camera view frame.
         *  @param [in] xyz Movement vector.
         */
        void MoveView(glm::dvec3 xyz);


        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // GET TRANSFORMATION MATRICES
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief Get the view matrix only.
         *  @return The 4x4 view matrix.
         */
        glm::dmat4 GetViewMatrix(void);

        /**
         *  @brief Get the projection matrix only.
         *  @return The 4x4 projection matrix depending on the @ref mode.
         */
        glm::dmat4 GetProjectionMatrix(void);

        /**
         *  @brief Get the projection-view matrix.
         *  @return 4x4 projection-view matrix.
         */
        glm::dmat4 GetProjectionViewMatrix(void);


        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Uniform Buffer Object Management
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief Generate the uniform buffer object.
         *  @param [in] bindingPoint The bining point for the uniform buffer object. Must be the same in the shader.
         *  @details The shader must use the following uniform block:
         *  layout (std140, binding = bindingPoint) uniform Camera{
         *        mat4 camProjectionView;
         *      mat4 camProjectionViewTFree;
         *        vec3 camPosition;
         *        vec3 camViewDirection;
         *    };
         *  @note The buffer will be initialized using the @ref UpdateUniformBufferObject member function.
         */
        void GenerateUniformBufferObject(GLuint bindingPoint);

        /**
         *  @brief Delete the uniform buffer object.
         */
        void DeleteUniformBufferObject(void);

        /**
         *  @brief Update the uniform buffer object.
         *  @details This will update the complete uniform block buffer data.
         */
        void UpdateUniformBufferObject(void);

    private:
        glm::dmat4 projectionMatrix; ///< The projection matrix. Use @ref UpdateProjectionMatrix to update the matrix.
        GLuint ubo;                  ///< The uniform buffer object.

}; /* class: Camera */


/**
 *  @brief Class: CouboidFrustumCuller
 *  @details Check whether a cuboid is visible by the cameras frustum or not.
 *  @note The cuboid must be aligned to the world space axes.
 */
class CuboidFrustumCuller {
    public:
        /**
         *  @brief Create a cuboid frustum culler using the cameras projection view matrix.
         *  @param [in] cameraProjectionViewMatrix The projection view matrix of the camera.
         */
        explicit CuboidFrustumCuller(const glm::mat4& cameraProjectionViewMatrix);

        /**
         *  @brief Delete the cuboid frustum culler.
         */
        ~CuboidFrustumCuller();

        /**
         *  @brief Check if a cuboid is visible.
         *  @param [in] blockLowestPosition The lowest position of the cuboid.
         *  @param [in] blockDimension The dimension of the cuboid.
         *  @return True if cuboid is visible, false otherwise.
         */
        bool IsVisible(glm::vec3 blockLowestPosition, glm::vec3 blockDimension);

    private:
        GLfloat cullInfo[6][4]; ///< Culling information (generated by constructor).

}; /* class: CuboidFrustumCuller */


/**
 *  @brief Class: Shader
 *  @details Handles vertex + geometry (optional) + fragment shader.
 */
class Shader {
    public:
        /**
         *  @brief Create a shader object.
         */
        Shader():id(0){}

        /**
         *  @brief Get the GLSL-version string to be used to generate shader.
         *  @return The version string, e.g. "450".
         *  @details Make sure that the OpenGL context is initialized to obtain correct version information.
         */
        static std::string GetShadingLanguageVersion(void);

        /**
         *  @brief Generate the shader.
         *  @param [in] fileName The filename of the GLSL shader file.
         *  @return True if success, false otherwise.
         *  @details A zero terminator will be added to the shader source. The GLSL version will be assigned automatically using the @ref GetShadingLanguageVersion function.
         */
        bool Generate(std::string fileName);

        /**
         *  @brief Generate the shader.
         *  @param [in] fileName The filename of the GLSL shader file.
         *  @param [in] version The version string number, e.g. "450".
         *  @return True if success, false otherwise.
         *  @details A zero terminator will be added to the shader source.
         */
        bool Generate(std::string fileName, std::string version);

        /**
         *  @brief Generate the shader.
         *  @param [in] fileName The filename of the GLSL shader file.
         *  @param [in] version The version string number, e.g. "450".
         *  @param [in] replacement Text replacement data.
         *  @return True if success, false otherwise.
         *  @details A zero terminator will be added to the shader source.
         */
        bool Generate(std::string fileName, std::string version, std::vector<std::pair<std::string, std::string>>& replacement);

        /**
         *  @brief Generate the shader.
         *  @param [in] fileData Binary file data.
         *  @param [in] version The version string number, e.g. "450".
         *  @return True if success, false otherwise.
         *  @details A zero terminator will be added to the shader source.
         */
        bool Generate(std::vector<uint8_t>& fileData, std::string version);

        /**
         *  @brief Generate the shader.
         *  @param [in] fileData Binary file data.
         *  @param [in] version The version string number, e.g. "450".
         *  @param [in] replacement Text replacement data.
         *  @return True if success, false otherwise.
         *  @details A zero terminator will be added to the shader source.
         */
        bool Generate(std::vector<uint8_t>& fileData, std::string version, std::vector<std::pair<std::string, std::string>>& replacement);

        /**
         *  @brief Delete the shader program.
         */
        void Delete(void);

        /**
         *  @brief Use the shader.
         */
        inline void Use(void){ DEBUG_GLCHECK( glUseProgram(id); ); }

        /**
         *  @brief Get the uniform location.
         *  @param [in] name Name of the uniform.
         *  @return Location of the uniform.
         */
        inline GLint GetUniformLocation(const GLchar* name){ return glGetUniformLocation(id, name); }

        /* The uniforms */
        inline void UniformMatrix4fv(const GLchar* name, GLboolean transpose, glm::mat4& matrix){ DEBUG_GLCHECK( glUniformMatrix4fv(glGetUniformLocation(id, name), 1, transpose, glm::value_ptr(matrix)); ); }
        inline void UniformMatrix4fv(GLint location, GLboolean transpose, glm::mat4& matrix){ DEBUG_GLCHECK( glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(matrix)); ); }
        inline void UniformMatrix4fv(const GLchar* name, GLboolean transpose, GLfloat* matrix){ DEBUG_GLCHECK( glUniformMatrix4fv(glGetUniformLocation(id, name), 1, transpose, matrix); ); }
        inline void UniformMatrix4fv(GLint location, GLboolean transpose, GLfloat* matrix){ DEBUG_GLCHECK( glUniformMatrix4fv(location, 1, transpose, matrix); ); }
        inline void UniformMatrix3fv(const GLchar* name, GLboolean transpose, glm::mat3& matrix){ DEBUG_GLCHECK( glUniformMatrix3fv(glGetUniformLocation(id, name), 1, transpose, glm::value_ptr(matrix)); ); }
        inline void UniformMatrix3fv(GLint location, GLboolean transpose, glm::mat3& matrix){ DEBUG_GLCHECK( glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(matrix)); ); }
        inline void UniformMatrix3fv(const GLchar* name, GLboolean transpose, GLfloat* matrix){ DEBUG_GLCHECK( glUniformMatrix3fv(glGetUniformLocation(id, name), 1, transpose, matrix); ); }
        inline void UniformMatrix3fv(GLint location, GLboolean transpose, GLfloat* matrix){ DEBUG_GLCHECK( glUniformMatrix3fv(location, 1, transpose, matrix); ); }
        inline void UniformMatrix2fv(const GLchar* name, GLboolean transpose, glm::mat2& matrix){ DEBUG_GLCHECK( glUniformMatrix2fv(glGetUniformLocation(id, name), 1, transpose, glm::value_ptr(matrix)); ); }
        inline void UniformMatrix2fv(GLint location, GLboolean transpose, glm::mat2& matrix){ DEBUG_GLCHECK( glUniformMatrix2fv(location, 1, transpose, glm::value_ptr(matrix)); ); }
        inline void UniformMatrix2fv(const GLchar* name, GLboolean transpose, GLfloat* matrix){ DEBUG_GLCHECK( glUniformMatrix2fv(glGetUniformLocation(id, name), 1, transpose, matrix); ); }
        inline void UniformMatrix2fv(GLint location, GLboolean transpose, GLfloat* matrix){ DEBUG_GLCHECK( glUniformMatrix2fv(location, 1, transpose, matrix); ); }
        inline void Uniform4f(const GLchar* name, glm::vec4& value){ DEBUG_GLCHECK( glUniform4f(glGetUniformLocation(id, name), value.x, value.y, value.z, value.w); ); }
        inline void Uniform4f(GLint location, glm::vec4& value){ DEBUG_GLCHECK( glUniform4f(location, value.x, value.y, value.z, value.w); ); }
        inline void Uniform4fv(const GLchar* name, GLsizei count, const GLfloat* value){ DEBUG_GLCHECK( glUniform4fv(glGetUniformLocation(id, name), count, value); ); }
        inline void Uniform4fv(GLint location, GLsizei count, const GLfloat* value){ DEBUG_GLCHECK( glUniform4fv(location, count, value); ); }
        inline void Uniform3f(const GLchar* name, glm::vec3& value){ DEBUG_GLCHECK( glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z); ); }
        inline void Uniform3f(GLint location, glm::vec3& value){ DEBUG_GLCHECK( glUniform3f(location, value.x, value.y, value.z); ); }
        inline void Uniform3fv(const GLchar* name, GLsizei count, const GLfloat* value){ DEBUG_GLCHECK( glUniform3fv(glGetUniformLocation(id, name), count, value); ); }
        inline void Uniform3fv(GLint location, GLsizei count, const GLfloat* value){ DEBUG_GLCHECK( glUniform3fv(location, count, value); ); }
        inline void Uniform2f(const GLchar* name, glm::vec2& value){ DEBUG_GLCHECK( glUniform2f(glGetUniformLocation(id, name), value.x, value.y); ); }
        inline void Uniform2f(GLint location, glm::vec2& value){ DEBUG_GLCHECK( glUniform2f(location, value.x, value.y); ); }
        inline void Uniform2fv(const GLchar* name, GLsizei count, const GLfloat* value){ DEBUG_GLCHECK( glUniform2fv(glGetUniformLocation(id, name), count, value); ); }
        inline void Uniform2fv(GLint location, GLsizei count, const GLfloat* value){ DEBUG_GLCHECK( glUniform2fv(location, count, value); ); }
        inline void Uniform1f(const GLchar* name, GLfloat value){ DEBUG_GLCHECK( glUniform1f(glGetUniformLocation(id, name), value); ); }
        inline void Uniform1f(GLint location, GLfloat value){ DEBUG_GLCHECK( glUniform1f(location, value); ); }
        inline void Uniform1fv(const GLchar* name, GLsizei count, const GLfloat* value){ DEBUG_GLCHECK( glUniform1fv(glGetUniformLocation(id, name), count, value); ); }
        inline void Uniform1fv(GLint location, GLsizei count, const GLfloat* value){ DEBUG_GLCHECK( glUniform1fv(location, count, value); ); }
        inline void Uniform4i(const GLchar* name, GLint value0, GLint value1, GLint value2, GLint value3){ DEBUG_GLCHECK( glUniform4i(glGetUniformLocation(id, name), value0, value1, value2, value3); ); }
        inline void Uniform4i(GLint location, GLint value0, GLint value1, GLint value2, GLint value3){ DEBUG_GLCHECK( glUniform4i(location, value0, value1, value2, value3); ); }
        inline void Uniform4iv(const GLchar* name, GLsizei count, const GLint* values){ DEBUG_GLCHECK( glUniform4iv(glGetUniformLocation(id, name), count, values); ); }
        inline void Uniform4iv(GLint location, GLsizei count, const GLint* values){ DEBUG_GLCHECK( glUniform4iv(location, count, values); ); }
        inline void Uniform3i(const GLchar* name, GLint value0, GLint value1, GLint value2){ DEBUG_GLCHECK( glUniform3i(glGetUniformLocation(id, name), value0, value1, value2); ); }
        inline void Uniform3i(GLint location, GLint value0, GLint value1, GLint value2){ DEBUG_GLCHECK( glUniform3i(location, value0, value1, value2); ); }
        inline void Uniform3iv(const GLchar* name, GLsizei count, const GLint* values){ DEBUG_GLCHECK( glUniform3iv(glGetUniformLocation(id, name), count, values); ); }
        inline void Uniform3iv(GLint location, GLsizei count, const GLint* values){ DEBUG_GLCHECK( glUniform3iv(location, count, values); ); }
        inline void Uniform2i(const GLchar* name, GLint value0, GLint value1){ DEBUG_GLCHECK( glUniform2i(glGetUniformLocation(id, name), value0, value1); ); }
        inline void Uniform2i(GLint location, GLint value0, GLint value1){ DEBUG_GLCHECK( glUniform2i(location, value0, value1); ); }
        inline void Uniform2iv(const GLchar* name, GLsizei count, const GLint* values){ DEBUG_GLCHECK( glUniform2iv(glGetUniformLocation(id, name), count, values); ); }
        inline void Uniform2iv(GLint location, GLsizei count, const GLint* values){ DEBUG_GLCHECK( glUniform2iv(location, count, values); ); }
        inline void Uniform1i(const GLchar* name, GLint value){ DEBUG_GLCHECK( glUniform1i(glGetUniformLocation(id, name), value); ); }
        inline void Uniform1i(GLint location, GLint value){ DEBUG_GLCHECK( glUniform1i(location, value); ); }
        inline void Uniform1ui(const GLchar* name, GLuint value){ DEBUG_GLCHECK( glUniform1ui(glGetUniformLocation(id, name), value); ); }
        inline void Uniform1ui(GLint location, GLuint value){ DEBUG_GLCHECK( glUniform1ui(location, value); ); }
        inline void Uniform1iv(const GLchar* name, GLsizei count, const GLint* values){ DEBUG_GLCHECK( glUniform1iv(glGetUniformLocation(id, name), count, values); ); }
        inline void Uniform1iv(GLint location, GLsizei count, const GLint* values){ DEBUG_GLCHECK( glUniform1iv(location, count, values); ); }
        inline void UniformBlockBinding(const GLchar* name, GLuint value){ DEBUG_GLCHECK( glUniformBlockBinding(id, glGetUniformBlockIndex(id, name), value); ); }
        inline void UniformBlockBinding(GLint location, GLuint value){ DEBUG_GLCHECK( glUniformBlockBinding(id, location, value); ); }

    private:
        GLuint id; ///< The program ID.

}; /* class: Shader */


} /* namespace: Engine */
} /* namespace: RD */

