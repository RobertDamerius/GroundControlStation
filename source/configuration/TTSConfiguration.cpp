#include <TTSConfiguration.hpp>
#include <Common.hpp>
#include <CFGParser.hpp>
#include <FileManager.hpp>


TTSConfiguration::TTSConfiguration(){
    systemPreString = "echo \"";
    systemPostString = "\"";
    enable = false;
}

void TTSConfiguration::Read(void){
    // Clear current configuration
    systemPreString = "echo \"";
    systemPostString = "\"";
    enable = false;

    // Read and parse the configuration file
    CFGParser cfg;
    uint32_t error = cfg.Parse(FileManager::GetName(FILE_CONFIGURATION_TTS));
    if(error){
        if(0xFFFFFFFF == error){
            GUILog(std::string("Text-to-speech configuration file not found!"), 255, 0, 0);
        }
        else{
            GUILog(std::string("Syntax error in line ") + std::to_string(error) + std::string(" of tts configuration file!"), 255, 0, 0);
        }
        return;
    }

    // Decode pre/post strings
    #ifdef _WIN32
    std::string keyPreString = "Windows.TextToSpeech.SystemPreString";
    std::string keyPostString = "Windows.TextToSpeech.SystemPostString";
    #else
    std::string keyPreString = "Linux.TextToSpeech.SystemPreString";
    std::string keyPostString = "Linux.TextToSpeech.SystemPostString";
    #endif
    auto got = cfg.configuration.find(keyPreString);
    if(got == cfg.configuration.end()){
        GUILog(keyPreString + std::string(" is not available in the tts configuration file! TTS is disabled!"), 255, 0, 0);
        return;
    }
    if(1 != got->second.size()){
        GUILog(keyPreString + std::string(" in the tts configuration file should only contain one value! TTS is disabled!"), 255, 0, 0);
        return;
    }
    std::string preString = got->second[0];
    got = cfg.configuration.find(keyPostString);
    if(got == cfg.configuration.end()){
        GUILog(keyPostString + std::string(" is not available in the tts configuration file! TTS is disabled!"), 255, 0, 0);
        return;
    }
    if(1 != got->second.size()){
        GUILog(keyPostString + std::string(" in the tts configuration file should only contain one value! TTS is disabled!"), 255, 0, 0);
        return;
    }
    std::string postString = got->second[0];

    // Decode enable flag
    bool ttsEnable = false;
    (void) cfg.FindSingleValue(ttsEnable, "TextToSpeech.Enable");

    // Set values
    systemPreString = preString;
    systemPostString = postString;
    enable = ttsEnable;
}

