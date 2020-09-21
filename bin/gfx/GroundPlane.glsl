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


uniform vec3 planeColor;


// Fragment shader main
void main(void){
    vec3 view = normalize(viewDir);

    // 1.0f if ground, 0.0f otherwise
    float groundMask = float((view.y * nearPlane.y) < 0.0f);

    // Calculate ground position
    vec3 groundPosition = nearPlane.xyz - (nearPlane.y/(groundMask*view.y + 1.0f - groundMask)) * view;

    // Create checker pattern
    float checkerScale1 = float((abs(int(floor(groundPosition.z))) ^ abs(int(floor(groundPosition.x)))) % 2);
    float checkerScale10 = float((abs(int(floor(groundPosition.z*0.1))) ^ abs(int(floor(groundPosition.x*0.1)))) % 2);
    float checkerScale100 = float((abs(int(floor(groundPosition.z*0.01))) ^ abs(int(floor(groundPosition.x*0.01)))) % 2);
    vec3 groundColor = planeColor*(groundMask*(checkerScale1*0.1f + 0.9f)*(checkerScale10*0.15f + 0.8f)*(checkerScale100*0.2f + 0.7f));

    // Alpha decreasing by distance
    float distance = sqrt(dot(groundPosition - nearPlane, groundPosition - nearPlane));
    float alpha = groundMask * (0.2f + 0.4f*smoothstep(1000.0f, 100.0f, distance));

    // Set fragment depth for ground plane
    vec4 p = camProjectionView * vec4(groundPosition.x, 0.0f, groundPosition.z, 1.0f);
    gl_FragDepth = gl_DepthRange.diff * ((p.z / p.w + 1.0f) / 2.0f) + gl_DepthRange.near;

    // Final color
    FragColor = vec4(groundColor, alpha);
}


#endif /* FRAGMENT_SHADER */

