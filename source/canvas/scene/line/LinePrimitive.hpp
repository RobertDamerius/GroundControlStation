#pragma once


// Line vertex type
#pragma pack(push, 1)
typedef struct {
    GLfloat position[3];
    GLfloat color[3];
} LineVertex;
#pragma pack(pop)


// Line primitive type
typedef enum {
    LINE_PRIMITIVE_TYPE_LINES,
    LINE_PRIMITIVE_TYPE_LINES_ADJACENCY,
    LINE_PRIMITIVE_TYPE_LINE_LOOP,
    LINE_PRIMITIVE_TYPE_LINE_STRIP,
    LINE_PRIMITIVE_TYPE_LINE_STRIP_ADJACENCY
} LineType;


class LinePrimitive {
    public:
        GLfloat width;  ///< Width of the line in pixels (default is 1.0).

        /**
         *  @brief Create the line primitive.
         *  @param [in] type Primitive type.
         */
        explicit LinePrimitive(LineType type);

        /**
         *  @brief Default copy constructor.
         */
        LinePrimitive(const LinePrimitive& l) = default;

        /**
         *  @brief Delete the line primitive.
         */
        virtual ~LinePrimitive();

        /**
         *  @brief Set the vertices.
         *  @param [in] v Container of vertices. The parameter may be changed by the call.
         */
        virtual void SetVertices(std::vector<LineVertex>& v) = 0;

        /**
         *  @brief Generate the line. This will generate GL content (VAO, VBO).
         *  @param [in] usage Specifies the expected usage pattern of the data store (default is GL_STATIC_DRAW).
         *  @details Calls the @ref Delete member function before actually generating the line.
         */
        void Generate(GLenum usage = GL_STATIC_DRAW);

        /**
         *  @brief Delete the generated GL content.
         */
        void Delete(void);

        /**
         *  @brief Draw the line.
         */
        void Draw(void);

        /* Operators */
        LinePrimitive& operator=(const LinePrimitive& rhs);

    protected:
        std::vector<LineVertex> vertices;

    private:
        GLenum mode;
        GLuint vao;
        GLuint vbo;
};

