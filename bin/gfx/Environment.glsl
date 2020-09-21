// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// VERTEX SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef VERTEX_SHADER

// Vertex shader input
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;


// Vertex shader output
out vec2 TexCoords;
out vec3 nearPlane;
out vec3 farPlane;
out vec3 viewDir;


// Camera uniform block
layout (std140, binding = $UBO_VIEW_CAMERA$) uniform Camera{
    mat4 camProjectionView;
    mat4 camProjectionViewTFree;
    vec3 camPosition;
    vec3 camViewDirection;
};


// Vertex shader main
void main(void){
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
    TexCoords = texCoords;
    mat4 invProjectionView = inverse(camProjectionView);

    // Get near and far clipping plane in world space coordinates
    vec4 n = invProjectionView * vec4(position.x, position.y, -1.0f, 1.0f);
    vec4 f = invProjectionView * vec4(position.x, position.y, 1.0f, 1.0f);
    nearPlane = (1.0 / n.w) * n.xyz;
    farPlane = (1.0 / f.w) * f.xyz;

    // Calculate view direction from near to far clipping plane
    viewDir = farPlane.xyz - nearPlane.xyz;
}

#endif /* VERTEX_SHADER */


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FRAGMENT SHADER
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef FRAGMENT_SHADER
#extension GL_EXT_texture_array : enable

// Fragment shader input
in vec2 TexCoords;
in vec3 nearPlane;
in vec3 farPlane;
in vec3 viewDir;


// Fragment shader output
layout (location = 0) out vec4 FragColor;


// Camera uniform block
layout (std140, binding = $UBO_VIEW_CAMERA$) uniform Camera{
    mat4 camProjectionView;
    mat4 camProjectionViewTFree;
    vec3 camPosition;
    vec3 camViewDirection;
};


// Uniforms
uniform samplerCube textureSkybox;
uniform sampler2D texturePositionDepth;
uniform sampler2D textureNormalSpecular;
uniform sampler2D textureAlbedoReflection;
uniform float ambientStrength;
uniform float shininess;

const vec3 lightDirection = normalize(vec3(2.0,-1.0,-0.5f));
const vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);


// Fragment shader main
void main(void){
    // Input textures (GBuffer and skybox)
    vec3 view = normalize(viewDir);
    vec3 position = texture(texturePositionDepth, TexCoords).rgb;
    vec3 normal = normalize(texture(textureNormalSpecular, TexCoords).rgb);
    vec3 albedo = texture(textureAlbedoReflection, TexCoords).rgb;
    float specular = texture(textureNormalSpecular, TexCoords).a;
    float depth = texture(texturePositionDepth, TexCoords).a;
    float reflection = texture(textureAlbedoReflection, TexCoords).a;
    vec3 skyboxView = texture(textureSkybox, view).rgb;
    vec3 skyboxReflection = texture(textureSkybox, reflect(view, normal)).rgb;

    // Blinn-Phong lighting + reflection (skybox)
    float n_dot_l = -dot(normal, lightDirection);
    vec3 halfwayDir = normalize(-view - lightDirection);
    vec3 objectColor = albedo * lightColor;
    vec3 ambientLight = ambientStrength * objectColor;
    vec3 diffuseLight = max(0.0f, n_dot_l) * objectColor;
    vec3 specularLight = specular * lightColor * pow(max(0.0f, dot(view, -reflect(lightDirection, normal))), shininess) * step(0.0, n_dot_l);
    vec3 reflectionLight = skyboxReflection * skyboxReflection;
    vec3 scene = mix(ambientLight + diffuseLight + specularLight, reflectionLight, reflection);

    // Skybox color, gamma-corrected
    vec3 skybox = skyboxView * skyboxView;

    // Final color
    vec3 finalColor = mix(skybox, scene, step(0.001f, depth));

    // Albedo and brightness
    FragColor = vec4(finalColor, 1.0f);
}


#endif /* FRAGMENT_SHADER */

