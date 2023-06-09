#pragma once


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Window settings
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define WINDOW_INITIAL_WIDTH    (1920)
#define WINDOW_INITIAL_HEIGHT   (1080)


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// OpenGL texture units
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Default texture unit
#define TEXTURENUM_DEFAULT                      (0)
#define TEXTUREUNIT_DEFAULT                     (GL_TEXTURE0 + TEXTURENUM_DEFAULT)
#define TEXTURENUM_FONT_SPRITE                  TEXTURENUM_DEFAULT
#define TEXTUREUNIT_FONT_SPRITE                 TEXTUREUNIT_DEFAULT

// GUI
#define TEXTURENUM_GUI                          (1)
#define TEXTUREUNIT_GUI                         (GL_TEXTURE0 + TEXTURENUM_GUI)

// Skybox
#define TEXTURENUM_SKYBOX                       (2)
#define TEXTUREUNIT_SKYBOX                      (GL_TEXTURE0 + TEXTURENUM_SKYBOX)

// G-buffer (Output textures)
#define TEXTURENUM_GBUFFER_POSITION_DEPTH       (3)
#define TEXTURENUM_GBUFFER_NORMAL_SPECULAR      (4)
#define TEXTURENUM_GBUFFER_ALBEDO_REFLECTION    (5)
#define TEXTUREUNIT_GBUFFER_POSITION_DEPTH      (GL_TEXTURE0 + TEXTURENUM_GBUFFER_POSITION_DEPTH)
#define TEXTUREUNIT_GBUFFER_NORMAL_SPECULAR     (GL_TEXTURE0 + TEXTURENUM_GBUFFER_NORMAL_SPECULAR)
#define TEXTUREUNIT_GBUFFER_ALBEDO_REFLECTION   (GL_TEXTURE0 + TEXTURENUM_GBUFFER_ALBEDO_REFLECTION)

// Environment (Output textures)
#define TEXTURENUM_ENVIRONMENT_ALBEDO           (6)
#define TEXTUREUNIT_ENVIRONMENT_ALBEDO          (GL_TEXTURE0 + TEXTURENUM_ENVIRONMENT_ALBEDO)


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Uniform buffer objects
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define UBO_NANO_GUI                            (0)   ///< For information purpose only!
#define UBO_VIEW_CAMERA                         (1)


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Additional rendering settings
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Gamma settings
#define GAMMA_DEFAULT                           (2.0f)
#define GAMMA_MIN                               (1.0f)
#define GAMMA_MAX                               (3.0f)

// Camera settings
#define CAMERA_CLIP_NEAR_DEFAULT                (0.1)
#define CAMERA_CLIP_FAR_DEFAULT                 (1000.0)
#define CAMERA_HORIZONTAL_WRAP                  (200.0)


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Version Settings
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern const std::string strAppName;
extern const std::string strVersion;
extern const std::string strBuilt;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Common functions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double GetTimestampUTC(void);
void GUILog(std::string msg, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);

