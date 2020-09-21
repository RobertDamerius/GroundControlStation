// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// VERTEX SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef VERTEX_SHADER

// Vertex shader input
layout (location = 0) in float valueA;
layout (location = 1) in float valueB;
layout (location = 2) in vec3 positionPrev;
layout (location = 3) in vec3 positionA;
layout (location = 4) in vec3 positionB;
layout (location = 5) in vec3 positionNext;

// Vertex shader output
out VS_OUT {
    float valueA;
    float valueB;
    vec4 positionPrev;
    vec4 positionA;
    vec4 positionB;
    vec4 positionNext;
} outputVS;

// Camera uniform block
layout (std140, binding = $UBO_VIEW_CAMERA$) uniform Camera{
    mat4 camProjectionView;
    mat4 camProjectionViewTFree;
    vec3 camPosition;
    vec3 camViewDirection;
};

// Uniforms
uniform mat4 modelMat;
uniform vec3 positionOffset;
uniform float altitudeGain;

// Vertex shader main
void main(void){
    outputVS.valueA = valueA;
    outputVS.valueB = valueB;
    outputVS.positionPrev = camProjectionView * modelMat * vec4(positionPrev.x + positionOffset.x, altitudeGain*(positionPrev.y + positionOffset.y), positionPrev.z + positionOffset.z, 1.0f);
    outputVS.positionA = camProjectionView * modelMat * vec4(positionA.x + positionOffset.x, altitudeGain*(positionA.y + positionOffset.y), positionA.z + positionOffset.z, 1.0f);
    outputVS.positionB = camProjectionView * modelMat * vec4(positionB.x + positionOffset.x, altitudeGain*(positionB.y + positionOffset.y), positionB.z + positionOffset.z, 1.0f);
    outputVS.positionNext = camProjectionView * modelMat * vec4(positionNext.x + positionOffset.x, altitudeGain*(positionNext.y + positionOffset.y), positionNext.z + positionOffset.z, 1.0f);
    gl_Position = outputVS.positionB;
}

#endif /* VERTEX_SHADER */


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GEOMETRY SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef GEOMETRY_SHADER

// Input and output primitives
layout (points) in;
layout (triangle_strip, max_vertices = 8) out;

// Input from vertex shader
in VS_OUT {
    float valueA;
    float valueB;
    vec4 positionPrev;
    vec4 positionA;
    vec4 positionB;
    vec4 positionNext;
} inputGS[];

// Output to fragment shader
out vec3 lineColor;

// Uniform
uniform float lineWidth;
uniform vec3 infoScreenSize; // x: aspect; y: invAspect; z: invHeight
uniform float valueScale;

// Convert vertex value to vertex color
vec3 ColorMapJet(float value){
    float red = 4.0f * (value - 0.375f) - step(0.75f, value) * 8.0f * (value - 0.75f);
    float green = 4.0f * (value - 0.125f) - step(0.5f, value) * 8.0f * (value - 0.5f);
    float blue = 4.0f * (value + 0.125f) - step(0.25f, value) * 8.0f * (value - 0.25f);
    return vec3(clamp(red, 0.0f, 1.0f), clamp(green, 0.0f, 1.0f), clamp(blue, 0.0f, 1.0f));
}

// Geometry shader main
void main(void){
    // Positions for line strip (A) - - (B)-----(C) - - (D)
    vec4 positionA = inputGS[0].positionPrev / max(0.0001f, inputGS[0].positionPrev.w);
    vec4 positionB = inputGS[0].positionA / max(0.0001f, inputGS[0].positionA.w);
    vec4 positionC = inputGS[0].positionB / max(0.0001f, inputGS[0].positionB.w);
    vec4 positionD = inputGS[0].positionNext / max(0.0001f, inputGS[0].positionNext.w);
    positionA.w = positionB.w = positionC.w = positionD.w = 1.0f;

    // Delta vectors for edges AB, BC, CD
    float f = lineWidth*infoScreenSize.z;
    vec4 deltaAB = vec4(f*normalize(vec2(positionA.y - positionB.y, (positionB.x - positionA.x)*infoScreenSize.x)), 0.0f, 0.0f);
    vec4 deltaBC = vec4(f*normalize(vec2(positionB.y - positionC.y, (positionC.x - positionB.x)*infoScreenSize.x)), 0.0f, 0.0f);
    vec4 deltaCD = vec4(f*normalize(vec2(positionC.y - positionD.y, (positionD.x - positionC.x)*infoScreenSize.x)), 0.0f, 0.0f);

    // Transform back to clip space
    deltaAB.x *= infoScreenSize.y;
    deltaBC.x *= infoScreenSize.y;
    deltaCD.x *= infoScreenSize.y;

    // Get line colors
    vec3 color1 = ColorMapJet(valueScale * inputGS[0].valueA);
    vec3 color2 = ColorMapJet(valueScale * inputGS[0].valueB);

    // Position 0
    lineColor = color1;
    gl_Position = positionB + deltaAB;
    EmitVertex();

    // Position 1
    lineColor = color1;
    gl_Position = positionB - deltaAB;
    EmitVertex();

    // Position 2
    lineColor = color1;
    gl_Position = positionB + deltaBC;
    EmitVertex();

    // Position 3
    lineColor = color1;
    gl_Position = positionB - deltaBC;
    EmitVertex();

    // Position 4
    lineColor = color2;
    gl_Position = positionC + deltaBC;
    EmitVertex();

    // Position 5
    lineColor = color2;
    gl_Position = positionC - deltaBC;
    EmitVertex();

    // Position 6
    lineColor = color2;
    gl_Position = positionC + deltaCD;
    EmitVertex();

    // Position 7
    lineColor = color2;
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

