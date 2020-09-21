// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// VERTEX SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef VERTEX_SHADER

// Vertex shader input
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

// Vertex shader output
out vec3 vertexColor;

// Camera uniform block
layout (std140, binding = $UBO_VIEW_CAMERA$) uniform Camera{
    mat4 camProjectionView;
    mat4 camProjectionViewTFree;
    vec3 camPosition;
    vec3 camViewDirection;
};

// Uniforms
uniform mat4 modelMat;
uniform vec3 colorMultiplier;

// Vertex shader main
void main(void){
    gl_Position = camProjectionView * modelMat * vec4(position, 1.0f);
    vertexColor = colorMultiplier * color;
}

#endif /* VERTEX_SHADER */


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GEOMETRY SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef GEOMETRY_SHADER

// Input and output primitives
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

// Input from vertex shader
in vec3 vertexColor[];

// Output to fragment shader
out vec3 lineColor;

// Uniform
uniform float lineWidth;
uniform vec3 infoScreenSize; // x: aspect; y: invAspect; z: invHeight

// Geometry shader main
void main(void){
    // Position A and B of line
    vec4 positionA = gl_in[0].gl_Position / max(0.0001f, gl_in[0].gl_Position.w);
    vec4 positionB = gl_in[1].gl_Position / max(0.0001f, gl_in[1].gl_Position.w);

    // Edge and normal direction from clip space
    vec2 edge = lineWidth*infoScreenSize.z*normalize(vec2((positionB.x - positionA.x)*infoScreenSize.x, positionB.y - positionA.y));
    vec2 normal = vec2(-edge.y, edge.x);

    // Delta vectors
    vec4 delta1 = vec4(normal + edge, 0.0f, 0.0f);
    vec4 delta2 = vec4(normal - edge, 0.0f, 0.0f);

    // Transform back to clip space
    delta1.x *= infoScreenSize.y;
    delta2.x *= infoScreenSize.y;

    // Position 0
    lineColor = vertexColor[0];
    gl_Position = positionA + delta2;
    EmitVertex();

    // Position 1
    lineColor = vertexColor[0];
    gl_Position = positionA - delta1;
    EmitVertex();

    // Position 2
    lineColor = vertexColor[1];
    gl_Position = positionB + delta1;
    EmitVertex();

    // Position 3
    lineColor = vertexColor[1];
    gl_Position = positionB - delta2;
    EmitVertex();

    // Primitive finished
    EndPrimitive();
}

#endif /* GEOMETRY_SHADER */


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FRAGMENT SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef FRAGMENT_SHADER

// Fragment shader input
in vec3 lineColor;

// Fragment shader output
layout (location = 0) out vec4 FragColor;

// Fragment shader main
void main(void){
    FragColor = vec4(lineColor, 1.0f);
}

#endif /* FRAGMENT_SHADER */

