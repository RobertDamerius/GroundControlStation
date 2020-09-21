#pragma once


#include <RD.hpp>


// Circular line buffer vertex type
#pragma pack(push, 1)
typedef struct {
    GLfloat valueA;
    GLfloat valueB;
    GLfloat positionPrev[3];
    GLfloat positionA[3];
    GLfloat positionB[3];
    GLfloat positionNext[3];
} CircularLineVertex;
#pragma pack(pop)


class CircularLineBuffer {
    public:
        GLfloat width;  ///< Width of the line in pixels (default is 1.0).

        /**
         *  @brief Create a circular line buffer instance.
         */
        CircularLineBuffer();

        /**
         *  @brief Default copy constructor.
         */
        CircularLineBuffer(const CircularLineBuffer& c) = default;

        /**
         *  @brief Delete the circular line buffer instance.
         */
        ~CircularLineBuffer();

        /**
         *  @brief Generate the GL content.
         *  @param [in] numLineSegments Number of line segments (buffer size). If this is value is zero it will be set to 1.
         *  @param [in] initialPosition Initial position value for all buffer entries, defaults to glm::vec3(0.0f).
         */
        void Generate(uint32_t numLineSegments, glm::vec3 initialPosition = glm::vec3(0.0f));

        /**
         *  @brief Delete and generate the GL content and resize the internal line segment buffer.
         *  @param [in] numLineSegments Number of line segments (buffer size). If this is value is zero it will be set to 1.
         */
        void Resize(uint32_t numLineSegments);

        /**
         *  @brief Delete the GL content.
         */
        void Delete(void);

        /**
         *  @brief Add a new point to the circular line buffer.
         *  @param [in] position World space position (OpenGL frame).
         *  @param [in] value Optional scalar value for new vertex.
         */
        void Add(glm::vec3 position, GLfloat value = 0.0);

        /**
         *  @brief Draw the line buffer.
         */
        void Draw(void);

        /* Operators */
        CircularLineBuffer& operator=(const CircularLineBuffer& rhs);

    protected:
        std::vector<CircularLineVertex> vertices;   ///< Internal line segment buffer.
        uint32_t index;                             ///< Index for the current line segment.

    private:
        GLuint vao;
        GLuint vbo;
};

