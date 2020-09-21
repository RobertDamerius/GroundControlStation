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
uniform vec3 positionOffset;
uniform float altitudeGain;

// Vertex shader main
void main(void){
    gl_Position = camProjectionView * modelMat * vec4(position.x + positionOffset.x, altitudeGain*(position.y + positionOffset.y), position.z + positionOffset.z, 1.0f);
    vertexColor = colorMultiplier * color;
}

#endif /* VERTEX_SHADER */


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GEOMETRY SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef GEOMETRY_SHADER

// Input and output primitives
layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 8) out;

// Input from vertex shader
in vec3 vertexColor[];

// Output to fragment shader
out vec3 lineColor;

// Uniform
uniform float lineWidth;
uniform vec3 infoScreenSize; // x: aspect; y: invAspect; z: invHeight

// Geometry shader main
void main(void){
    // Positions for line strip (A) - - (B)-----(C) - - (D)
    vec4 positionA = gl_in[0].gl_Position / max(0.0001f, gl_in[0].gl_Position.w);
    vec4 positionB = gl_in[1].gl_Position / max(0.0001f, gl_in[1].gl_Position.w);
    vec4 positionC = gl_in[2].gl_Position / max(0.0001f, gl_in[2].gl_Position.w);
    vec4 positionD = gl_in[3].gl_Position / max(0.0001f, gl_in[3].gl_Position.w);

    // Delta vectors for edges AB, BC, CD
    float f = lineWidth*infoScreenSize.z;
    vec4 deltaAB = vec4(f*normalize(vec2(positionA.y - positionB.y, (positionB.x - positionA.x)*infoScreenSize.x)), 0.0f, 0.0f);
    vec4 deltaBC = vec4(f*normalize(vec2(positionB.y - positionC.y, (positionC.x - positionB.x)*infoScreenSize.x)), 0.0f, 0.0f);
    vec4 deltaCD = vec4(f*normalize(vec2(positionC.y - positionD.y, (positionD.x - positionC.x)*infoScreenSize.x)), 0.0f, 0.0f);

    // Transform back to clip space
    deltaAB.x *= infoScreenSize.y;
    deltaBC.x *= infoScreenSize.y;
    deltaCD.x *= infoScreenSize.y;

    // Position 0
    lineColor = vertexColor[1];
    gl_Position = positionB + deltaAB;
    EmitVertex();

    // Position 1
    lineColor = vertexColor[1];
    gl_Position = positionB - deltaAB;
    EmitVertex();

    // Position 2
    lineColor = vertexColor[1];
    gl_Position = positionB + deltaBC;
    EmitVertex();

    // Position 3
    lineColor = vertexColor[1];
    gl_Position = positionB - deltaBC;
    EmitVertex();

    // Position 4
    lineColor = vertexColor[2];
    gl_Position = positionC + deltaBC;
    EmitVertex();

    // Position 5
    lineColor = vertexColor[2];
    gl_Position = positionC - deltaBC;
    EmitVertex();

    // Position 6
    lineColor = vertexColor[2];
    gl_Position = positionC + deltaCD;
    EmitVertex();

    // Position 7
    lineColor = vertexColor[2];
    gl_Position = positionC - deltaCD;
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

