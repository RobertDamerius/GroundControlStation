// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// VERTEX SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef VERTEX_SHADER

// Vertex shader input
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;


// Vertex shader output
out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec3 color;
} vsOut;


// Uniforms
uniform mat4 modelMat;
uniform vec3 colorMultiplier;
uniform uint classification;
uniform vec3 velocity;


// Camera uniform block
layout (std140, binding = $UBO_VIEW_CAMERA$) uniform Camera{
    mat4 camProjectionView;
    mat4 camProjectionViewTFree;
    vec3 camPosition;
    vec3 camViewDirection;
};


// Vertex shader main
void main(void){
    // World space position
    vec4 pos = modelMat * vec4(position, 1.0f);

    // Object color
    vec3 objColor = colorMultiplier * color;

    // Vertex shader output to fragment shader
    vsOut.fragPos = pos.xyz;
    vsOut.normal = vec3(modelMat * vec4(normal, 0.0f));
    vsOut.color = objColor * objColor;

    // Resulting position on screen
    gl_Position = camProjectionView * pos;
}

#endif /* VERTEX_SHADER */


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FRAGMENT SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef FRAGMENT_SHADER

// Fragment shader input
in VS_OUT{
    vec3 fragPos;
    vec3 normal;
    vec3 color;
} fsIn;


// Fragment shader output
layout (location = 0) out vec4 outPositionDepth;
layout (location = 1) out vec4 outNormalSpec;
layout (location = 2) out vec4 outAlbedoReflection;


// Camera uniform block
layout (std140, binding = $UBO_VIEW_CAMERA$) uniform Camera{
    mat4 camProjectionView;
    mat4 camProjectionViewTFree;
    vec3 camPosition;
    vec3 camViewDirection;
};


// Uniforms
uniform float specular;
uniform float reflection;
uniform uint classification;
uniform vec3 velocity;


// Fragment shader main
void main(void){
    // Output colors
    outPositionDepth = vec4(fsIn.fragPos, sqrt(dot(fsIn.fragPos - camPosition, fsIn.fragPos - camPosition)));
    outNormalSpec = vec4(normalize(fsIn.normal), specular);
    outAlbedoReflection = vec4(fsIn.color, reflection);
}


#endif /* FRAGMENT_SHADER */

