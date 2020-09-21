// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// VERTEX SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef VERTEX_SHADER

// Vertex shader input
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

// Vertex shader output
out vec2 TexCoords;

// Uniforms
uniform vec2 invResolutionHalf;
uniform vec2 offsetClipSpace;

// Vertex shader main
void main(void){
    gl_Position = vec4(offsetClipSpace.x + invResolutionHalf.x*position.x, offsetClipSpace.y + invResolutionHalf.y*position.y, -1.0f, 1.0f);
    TexCoords = texCoords;
}

#endif /* VERTEX_SHADER */


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FRAGMENT SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef FRAGMENT_SHADER

// Fragment shader input
in vec2 TexCoords;

// Fragment shader output
layout (location = 0) out vec4 FragColor;

// Uniforms
uniform sampler2D textureFontSprite;
uniform vec4 color;

// Fragment shader main
void main(void){
    float alpha = texture(textureFontSprite, TexCoords).r;
    FragColor = vec4(color.rgb, alpha * color.a);
}

#endif /* FRAGMENT_SHADER */

