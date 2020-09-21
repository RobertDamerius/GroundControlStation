#include <Renderer.hpp>
#include <Common.hpp>
#include <AppWindow.hpp>
#include <Configuration.hpp>


Renderer::Renderer(){
    this->gamma = GAMMA_DEFAULT;
}

Renderer::~Renderer(){}

bool Renderer::Initialize(void){
    // Make sure that the renderer is terminated
        Terminate();

    // Generate framebuffers
        glm::ivec2 windowSize = AppWindow::MinSize(AppWindow::GetSize());
        if(!GenerateFrameBuffers(windowSize.x, windowSize.y)){
            goto error;
        }

    // Generate shaders
        if(!GenerateShaders()){
            goto error;
        }

    // Return success
        DEBUG_GLCHECK( glActiveTexture(TEXTUREUNIT_DEFAULT); );
        this->gamma = Configuration::style.displayGamma;
        return true;

    // Error handling
        error:
        Terminate();
        return false;
}

void Renderer::Terminate(void){
    DeleteShaders();
    DeleteFrameBuffers();
}

void Renderer::CallbackFramebufferResize(GLFWwindow* wnd, glm::ivec2 size){
    size = AppWindow::MinSize(size);
    GenerateFrameBuffers(size.x, size.y);
    DEBUG_GLCHECK( glViewport(0, 0, size.x, size.y); );
    shaderEnvironment.Use();
    shaderEnvironment.SetResolution(size);
    shaderSceneText.Use();
    shaderSceneText.SetResolution(size);
    shaderLine.Use();
    shaderLine.SetResolution(size);
    shaderLineStrip.Use();
    shaderLineStrip.SetResolution(size);
    shaderCircularLineBuffer.Use();
    shaderCircularLineBuffer.SetResolution(size);
    (void)wnd;
}

void Renderer::Render(Scene& scene, GUI& gui){
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Render GUI
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        DEBUG_GLCHECK( glViewport(0, 0, fbGUI.width, fbGUI.height); );
        DEBUG_GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, fbGUI.fbo); );
        DEBUG_GLCHECK( glClear(GL_COLOR_BUFFER_BIT); );
        gui.Render();
        DEBUG_GLCHECK( glEnable(GL_CULL_FACE); );
        DEBUG_GLCHECK( glCullFace(GL_BACK); );
        DEBUG_GLCHECK( glDisable(GL_BLEND); );

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Render scene to Gbuffer
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        DEBUG_GLCHECK( glViewport(0, 0, fbGBuffer.width, fbGBuffer.height); );
        DEBUG_GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, fbGBuffer.fbo); );
        DEBUG_GLCHECK( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); );
        DEBUG_GLCHECK( glEnable(GL_DEPTH_TEST); );
        this->RenderToGBuffer(scene);
        DEBUG_GLCHECK( glDisable(GL_DEPTH_TEST); );

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Environmental rendering
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        DEBUG_GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, fbEnvironment.fbo); );
        DEBUG_GLCHECK( glClear(GL_COLOR_BUFFER_BIT); );
        shaderEnvironment.Use();
        AppWindow::DrawScreenQuad();
        DEBUG_GLCHECK( glEnable(GL_DEPTH_TEST); );
        DEBUG_GLCHECK( glEnable(GL_BLEND); );
        DEBUG_GLCHECK( glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); );
        DEBUG_GLCHECK( glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD); );
        DEBUG_GLCHECK( glDepthMask(GL_FALSE); );
        this->RenderTransparencyParts(scene);
        DEBUG_GLCHECK( glDepthMask(GL_TRUE); );
        DEBUG_GLCHECK( glDisable(GL_BLEND); );
        DEBUG_GLCHECK( glDisable(GL_DEPTH_TEST); );

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Post processing, render to default framebuffer (0)
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        DEBUG_GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, 0); );
        shaderPostProcessing.Use();
        AppWindow::DrawScreenQuad();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Use accumulation buffer for a slight motion-blur effect
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // glAccum(GL_MULT, 0.1);
        // glAccum(GL_ACCUM, 0.9);
        // glAccum(GL_RETURN, 1.0);
}

void Renderer::BindGUIFramebuffer(void){
    DEBUG_GLCHECK( glViewport(0, 0, fbGUI.width, fbGUI.height); );
    DEBUG_GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, fbGUI.fbo); );
}

void Renderer::SetGamma(GLfloat gamma){
    shaderPostProcessing.Use();
    this->gamma = shaderPostProcessing.SetGamma(gamma);
}

bool Renderer::GenerateFrameBuffers(int width, int height){
    // (Re-)generate framebuffers
        if(!fbGUI.Generate(width, height, TEXTUREUNIT_GUI)){
            LogError("Could not generate GUI FBO.\n");
            return false;
        }
        if(!fbGBuffer.Generate(width, height, TEXTUREUNIT_GBUFFER_POSITION_DEPTH, TEXTUREUNIT_GBUFFER_NORMAL_SPECULAR, TEXTUREUNIT_GBUFFER_ALBEDO_REFLECTION)){
            LogError("Could not generate G-buffer FBO.\n");
            return false;
        }
        if(!fbEnvironment.Generate(width, height, fbGBuffer.rbo, TEXTUREUNIT_ENVIRONMENT_ALBEDO)){
            LogError("Could not generate environment FBO.\n");
            return false;
        }

    // Bind texture for respective (reserved) texture unit
        DEBUG_GLCHECK( glActiveTexture(TEXTUREUNIT_GUI); );
        DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, fbGUI.cbo); );
        DEBUG_GLCHECK( glActiveTexture(TEXTUREUNIT_GBUFFER_POSITION_DEPTH); );
        DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, fbGBuffer.cboPositionDepth); );
        DEBUG_GLCHECK( glActiveTexture(TEXTUREUNIT_GBUFFER_NORMAL_SPECULAR); );
        DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, fbGBuffer.cboNormalSpecular); );
        DEBUG_GLCHECK( glActiveTexture(TEXTUREUNIT_GBUFFER_ALBEDO_REFLECTION); );
        DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, fbGBuffer.cboAlbedoReflection); );
        DEBUG_GLCHECK( glActiveTexture(TEXTUREUNIT_ENVIRONMENT_ALBEDO); );
        DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, fbEnvironment.cbo); );

    // Set default texture unit
        DEBUG_GLCHECK( glActiveTexture(TEXTUREUNIT_DEFAULT); );

    return true;
}

void Renderer::DeleteFrameBuffers(void){
    fbEnvironment.Delete();
    fbGBuffer.Delete();
    fbGUI.Delete();
}

bool Renderer::GenerateShaders(void){
    if(!shaderVehicle.Generate()){
        LogError("Could not create vehicle shader!\n");
        return false;
    }
    if(!shaderTransparentVehicle.Generate()){
        LogError("Could not create transparent vehicle shader!\n");
        return false;
    }
    if(!shaderGroundPlane.Generate()){
        LogError("Could not create ground plane shader!\n");
        return false;
    }
    if(!shaderEnvironment.Generate()){
        LogError("Could not create environment shader!\n");
        return false;
    }
    if(!shaderPostProcessing.Generate()){
        LogError("Could not create post processing shader!\n");
        return false;
    }
    if(!shaderSceneText.Generate()){
        LogError("Could not create scene text shader!\n");
        return false;
    }
    if(!shaderLine.Generate()){
        LogError("Could not create line shader!\n");
        return false;
    }
    if(!shaderLineStrip.Generate()){
        LogError("Could not create line strip shader!\n");
        return false;
    }
    if(!shaderCircularLineBuffer.Generate()){
        LogError("Could not create circular line buffer shader!\n");
        return false;
    }
    shaderPostProcessing.Use();
    this->gamma = shaderPostProcessing.SetGamma(this->gamma);
    return true;
}

void Renderer::DeleteShaders(void){
    shaderCircularLineBuffer.Delete();
    shaderLineStrip.Delete();
    shaderLine.Delete();
    shaderSceneText.Delete();
    shaderPostProcessing.Delete();
    shaderEnvironment.Delete();
    shaderGroundPlane.Delete();
    shaderTransparentVehicle.Delete();
    shaderVehicle.Delete();
}

void Renderer::RenderToGBuffer(Scene& scene){
    shaderVehicle.Use();
    scene.vehicleManager.Render(shaderVehicle);
}

void Renderer::RenderTransparencyParts(Scene& scene){
    // Render ground plance
    shaderGroundPlane.Use();
    scene.groundPlane.Draw();

    // Render lines
    DEBUG_GLCHECK( glDepthMask(GL_TRUE); );
    scene.vehicleManager.RenderLines(shaderTransparentVehicle, shaderLine, shaderLineStrip, shaderCircularLineBuffer);
    DEBUG_GLCHECK( glDepthMask(GL_FALSE); );

    // Render all scene texts
    DEBUG_GLCHECK( glActiveTexture(TEXTUREUNIT_FONT_SPRITE); );
    shaderSceneText.Use();
    scene.vehicleManager.RenderLabels(shaderSceneText);
}

