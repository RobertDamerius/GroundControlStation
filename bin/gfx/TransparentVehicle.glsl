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
    vec3 color;
} vsOut;


// Uniforms
uniform mat4 modelMat;
uniform vec3 colorMultiplier;


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
    vec3 color;
} fsIn;


// Fragment shader output
layout (location = 0) out vec4 FragColor;


// Uniforms
uniform float alpha;


// Fragment shader main
void main(void){
    // Output colors
    FragColor = vec4(fsIn.color, alpha);
}


#endif /* FRAGMENT_SHADER */

