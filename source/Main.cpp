#include <Common.hpp>
#include <FileManager.hpp>
#include <GLWindow.hpp>
#include <AppWindow.hpp>
#include <NetworkManager.hpp>
#include <RD.hpp>
#include <Test.hpp>

//#TODO: Generate VBOs (trajectory, waypoints) around 0.0

int main(int argc, char **argv){
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Initialize and create application window
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        FileManager::Initialize(argv[0]);
        if(!GLWindow::Initialize(&appWindow, WINDOW_INITIAL_WIDTH, WINDOW_INITIAL_HEIGHT, strAppName, false)){
            GLWindow::Terminate();
            return -1;
        }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Show information about system and application
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Console::Message(stdout,"%s\n\nVersion:        %s\nBuilt:          %s\nDate (UTC):     %s\nArguments:     ", strAppName.c_str(), strVersion.c_str(), strBuilt.c_str(), RD::Core::GetTimeStringUTC().c_str());
        for(int i = 1; i < argc; i++)
            Console::Message(stdout," [%s]", argv[i]);
        Console::Message(stdout,"\n\n");
        Console::Message(stdout,"GLFW-Version:   %s\n", glfwGetVersionString());
        std::string versionGL, versionGLSL, vendor, renderer;
        GetGLInfo(&versionGL, &versionGLSL, &vendor, &renderer);
        Console::Message(stdout,"GL-Version:     %s\nGLSL-Version:   %s\nGL-Vendor:      %s\nGL-Renderer:    %s\n\n", versionGL.c_str(), versionGLSL.c_str(), vendor.c_str(), renderer.c_str());

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Check for test mode
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool testMode = (argc > 1) && !std::string(argv[1]).compare("--test");

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Main loop
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if(testMode) Test::Start();
        NetworkManager::Start();
        GLWindow::MainLoop();
        NetworkManager::Stop();
        if(testMode) Test::Stop();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Cleanup
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        GLWindow::Terminate();
        return 0;
}

