#include <Common.hpp>
#include <GCSConfiguration.hpp>
#include <FileManager.hpp>
#include <nlohmann/json.hpp>


GCSConfiguration::GCSConfiguration(){
    Clear();
}

void GCSConfiguration::Clear(void){
    window.width = 1920;
    window.height = 1080;
    window.fullscreen = false;
    window.fullscreenMonitorIndex = 0;
    window.resizable = true;
    window.visible = true;
    window.decorated = true;
    window.focused = true;
    window.maximized = false;
    window.glMajorVersion = 4;
    window.glMinorVersion = 5;
    textToSpeech.enable = true;
    textToSpeech.windows.systemPreString = "PowerShell -windowstyle hidden -Command \"Add-Type -AssemblyName System.Speech; $A = New-Object System.Speech.Synthesis.SpeechSynthesizer; $A.SelectVoice('Microsoft Zira Desktop'); $A.Speak('";
    textToSpeech.windows.systemPostString = "');\"";
    textToSpeech.linuxOS.systemPreString = "spd-say -w -l EN -t female2 \"";
    textToSpeech.linuxOS.systemPostString = "\"";
    view.geographicOrigin.useFromFirstReceivedNavigationMessage = true;
    view.geographicOrigin.latitude = 0.0;
    view.geographicOrigin.longitude = 0.0;
    view.geographicOrigin.altitude = 0.0;
    view.camera.enable3D = false;
    view.camera.clipNear = CAMERA_CLIP_NEAR_DEFAULT;
    view.camera.clipFar = CAMERA_CLIP_FAR_DEFAULT;
    view.display.gamma = GAMMA_DEFAULT;
    view.display.groundColor = {134,169,182};
    scene.ambient = 0.1;
    scene.specularity = 1.0;
    scene.shininess = 32.0;
    scene.reflection = 0.1;
    defaultVehicleStyle.altitudeToZero = false;
    defaultVehicleStyle.positionHistory.enable = false;
    defaultVehicleStyle.positionHistory.maximumSOG = 2.0;
    defaultVehicleStyle.positionHistory.lineWidth = 2.0;
    defaultVehicleStyle.positionHistory.updatePeriodMs = 20.0;
    defaultVehicleStyle.positionHistory.bufferSize = 100;
    defaultVehicleStyle.waypoints.enable = true;
    defaultVehicleStyle.waypoints.color = {0, 255, 0};
    defaultVehicleStyle.waypoints.vehicleColor = {255, 255, 255};
    defaultVehicleStyle.waypoints.vehicleAlpha = 0.5;
    defaultVehicleStyle.waypoints.lineWidth = 2.0;
    defaultVehicleStyle.waypoints.symbolSize = 1.0;
    defaultVehicleStyle.trajectory.enable = true;
    defaultVehicleStyle.trajectory.color = {255, 0, 255};
    defaultVehicleStyle.trajectory.vehicleColor = {255, 255, 255};
    defaultVehicleStyle.trajectory.vehicleAlpha = 0.5;
    defaultVehicleStyle.trajectory.vehicleStride = 1.0;
    defaultVehicleStyle.trajectory.lineWidth = 2.0;
    defaultVehicleStyle.compass.enable = true;
    defaultVehicleStyle.compass.radius = 10.0;
    defaultVehicleStyle.compass.color = {255, 255, 255};
    defaultVehicleStyle.polygons.enable = true;
    defaultVehicleStyle.polygons.upperLimit = 1.0;
    defaultVehicleStyle.polygons.lowerLimit = -1.0;
    defaultVehicleStyle.polygons.color = {255, 255, 255};
    network.unicast.clear();
    network.multicast.clear();
    network.joystick.clear();
    initialDatabase.clear();
    // optional default values:
    // "joystick": [
    //     "0>127.0.0.1:12701@0.0.0.0"
    // ],
    // "multicast": [
    //     "239.0.0.71:23971@0.0.0.0",
    //     "239.0.0.76:23976@0.0.0.0",
    //     "239.0.0.78:23978@0.0.0.0",
    //     "239.0.0.80:23980@0.0.0.0",
    //     "239.0.0.84:23984@0.0.0.0"
    // ],
    // "unicast": [
    //     "23971@0.0.0.0",
    //     "23976@0.0.0.0",
    //     "23978@0.0.0.0",
    //     "23980@0.0.0.0",
    //     "23984@0.0.0.0"
    // ]
}

void GCSConfiguration::Read(void){
    // set default values
    Clear();

    // parse JSON file
    std::string filename = FileManager::GetName(FILE_CONFIGURATION_GCS);
    nlohmann::json jsonData;
    try{
        std::ifstream configurationFile(filename);
        jsonData = nlohmann::json::parse(configurationFile);
    }
    catch(const std::exception& e){
        GUILog(std::string("Failed to parse configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        GUILog(std::string(e.what()), 255, 0, 0);
        return;
    }

    // read configuration values and print errors if format is invalid
    std::vector<std::string> networkUnicast, networkMulticast, networkJoystick, initialDB;
    try{ window.width                                                  = jsonData.at("window").at("width");                                                                        } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ window.height                                                 = jsonData.at("window").at("height");                                                                       } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ window.fullscreen                                             = jsonData.at("window").at("fullscreen");                                                                   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ window.fullscreenMonitorIndex                                 = jsonData.at("window").at("fullscreenMonitorIndex");                                                       } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ window.resizable                                              = jsonData.at("window").at("resizable");                                                                    } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ window.visible                                                = jsonData.at("window").at("visible");                                                                      } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ window.decorated                                              = jsonData.at("window").at("decorated");                                                                    } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ window.focused                                                = jsonData.at("window").at("focused");                                                                      } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ window.maximized                                              = jsonData.at("window").at("maximized");                                                                    } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ window.glMajorVersion                                         = jsonData.at("window").at("glMajorVersion");                                                               } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ window.glMinorVersion                                         = jsonData.at("window").at("glMinorVersion");                                                               } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ textToSpeech.enable                                           = jsonData.at("ui").at("textToSpeech").at("enable");                                                        } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ textToSpeech.windows.systemPreString                          = jsonData.at("ui").at("textToSpeech").at("windows").at("systemPreString");                                 } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ textToSpeech.windows.systemPostString                         = jsonData.at("ui").at("textToSpeech").at("windows").at("systemPostString");                                } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ textToSpeech.linuxOS.systemPreString                          = jsonData.at("ui").at("textToSpeech").at("linux").at("systemPreString");                                   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ textToSpeech.linuxOS.systemPostString                         = jsonData.at("ui").at("textToSpeech").at("linux").at("systemPostString");                                  } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ view.geographicOrigin.useFromFirstReceivedNavigationMessage   = jsonData.at("ui").at("view").at("geographicOrigin").at("useFromFirstReceivedNavigationMessage");          } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ view.geographicOrigin.latitude                                = 0.0174532925199433 * jsonData.at("ui").at("view").at("geographicOrigin").at("latitude").get<double>();    } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ view.geographicOrigin.longitude                               = 0.0174532925199433 * jsonData.at("ui").at("view").at("geographicOrigin").at("longitude").get<double>();   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ view.geographicOrigin.altitude                                = jsonData.at("ui").at("view").at("geographicOrigin").at("altitude");                                       } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ view.camera.enable3D                                          = jsonData.at("ui").at("view").at("camera").at("enable3D");                                                 } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ view.camera.clipNear                                          = jsonData.at("ui").at("view").at("camera").at("clipNear");                                                 } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ view.camera.clipFar                                           = jsonData.at("ui").at("view").at("camera").at("clipFar");                                                  } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ view.display.gamma                                            = jsonData.at("ui").at("view").at("display").at("gamma");                                                   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ view.display.groundColor                                      = jsonData.at("ui").at("view").at("display").at("groundColor");                                             } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ scene.ambient                                                 = jsonData.at("ui").at("scene").at("ambient");                                                              } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ scene.specularity                                             = jsonData.at("ui").at("scene").at("specularity");                                                          } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ scene.shininess                                               = jsonData.at("ui").at("scene").at("shininess");                                                            } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ scene.reflection                                              = jsonData.at("ui").at("scene").at("reflection");                                                           } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.altitudeToZero                            = jsonData.at("ui").at("defaultVehicleStyle").at("altitudeToZero");                                         } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.positionHistory.enable                    = jsonData.at("ui").at("defaultVehicleStyle").at("positionHistory").at("enable");                           } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.positionHistory.maximumSOG                = jsonData.at("ui").at("defaultVehicleStyle").at("positionHistory").at("maximumSOG");                       } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.positionHistory.lineWidth                 = jsonData.at("ui").at("defaultVehicleStyle").at("positionHistory").at("lineWidth");                        } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.positionHistory.updatePeriodMs            = jsonData.at("ui").at("defaultVehicleStyle").at("positionHistory").at("updatePeriodMs");                   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.positionHistory.bufferSize                = jsonData.at("ui").at("defaultVehicleStyle").at("positionHistory").at("bufferSize");                       } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.waypoints.enable                          = jsonData.at("ui").at("defaultVehicleStyle").at("waypoints").at("enable");                                 } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.waypoints.color                           = jsonData.at("ui").at("defaultVehicleStyle").at("waypoints").at("color");                                  } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.waypoints.vehicleColor                    = jsonData.at("ui").at("defaultVehicleStyle").at("waypoints").at("vehicleColor");                           } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.waypoints.vehicleAlpha                    = jsonData.at("ui").at("defaultVehicleStyle").at("waypoints").at("vehicleAlpha");                           } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.waypoints.lineWidth                       = jsonData.at("ui").at("defaultVehicleStyle").at("waypoints").at("lineWidth");                              } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.waypoints.symbolSize                      = jsonData.at("ui").at("defaultVehicleStyle").at("waypoints").at("symbolSize");                             } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.trajectory.enable                         = jsonData.at("ui").at("defaultVehicleStyle").at("trajectory").at("enable");                                } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.trajectory.color                          = jsonData.at("ui").at("defaultVehicleStyle").at("trajectory").at("color");                                 } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.trajectory.vehicleColor                   = jsonData.at("ui").at("defaultVehicleStyle").at("trajectory").at("vehicleColor");                          } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.trajectory.vehicleAlpha                   = jsonData.at("ui").at("defaultVehicleStyle").at("trajectory").at("vehicleAlpha");                          } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.trajectory.vehicleStride                  = jsonData.at("ui").at("defaultVehicleStyle").at("trajectory").at("vehicleStride");                         } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.trajectory.lineWidth                      = jsonData.at("ui").at("defaultVehicleStyle").at("trajectory").at("lineWidth");                             } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.compass.enable                            = jsonData.at("ui").at("defaultVehicleStyle").at("compass").at("enable");                                   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.compass.radius                            = jsonData.at("ui").at("defaultVehicleStyle").at("compass").at("radius");                                   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.compass.color                             = jsonData.at("ui").at("defaultVehicleStyle").at("compass").at("color");                                    } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.polygons.enable                           = jsonData.at("ui").at("defaultVehicleStyle").at("polygons").at("enable");                                  } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.polygons.upperLimit                       = jsonData.at("ui").at("defaultVehicleStyle").at("polygons").at("upperLimit");                              } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.polygons.lowerLimit                       = jsonData.at("ui").at("defaultVehicleStyle").at("polygons").at("lowerLimit");                              } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ defaultVehicleStyle.polygons.color                            = jsonData.at("ui").at("defaultVehicleStyle").at("polygons").at("color");                                   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ networkUnicast                                                = jsonData.at("network").at("unicast");                                                                     } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ networkMulticast                                              = jsonData.at("network").at("multicast");                                                                   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ networkJoystick                                               = jsonData.at("network").at("joystick");                                                                    } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
    try{ initialDB                                                     = jsonData.at("initialDatabase");                                                                           } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }

    // make network configuration and initialDB unique
    std::sort(networkUnicast.begin(), networkUnicast.end());
    std::sort(networkMulticast.begin(), networkMulticast.end());
    std::sort(networkJoystick.begin(), networkJoystick.end());
    std::sort(initialDB.begin(), initialDB.end());
    networkUnicast.erase(std::unique(networkUnicast.begin(), networkUnicast.end()), networkUnicast.end());
    networkMulticast.erase(std::unique(networkMulticast.begin(), networkMulticast.end()), networkMulticast.end());
    networkJoystick.erase(std::unique(networkJoystick.begin(), networkJoystick.end()), networkJoystick.end());
    initialDB.erase(std::unique(initialDB.begin(), initialDB.end()), initialDB.end());

    // deserialize network configuration
    for(auto&& str : networkUnicast){
        unsigned p = 0, a = 0, b = 0, c = 0, d = 0;
        int num;
        int r = sscanf(str.c_str(),"%u@%u.%u.%u.%u%n",&p,&a,&b,&c,&d,&num);
        if((5 != r) || ((int)str.size() != num) || (p > 65535) || (a > 255) || (b > 255) || (c > 255) || (d > 255)){
            GUILog(std::string("Invalid unicast configuration \"") + str + std::string("\"!"), 255, 0, 0);
            continue;
        }
        network.unicast.push_back(unicast_configuration());
        network.unicast.back().port = (uint16_t)p;
        network.unicast.back().ipInterface[0] = (uint8_t)a;
        network.unicast.back().ipInterface[1] = (uint8_t)b;
        network.unicast.back().ipInterface[2] = (uint8_t)c;
        network.unicast.back().ipInterface[3] = (uint8_t)d;
    }
    for(auto&& str : networkMulticast){
        unsigned w = 0, x = 0, y = 0, z = 0, p = 0, a = 0, b = 0, c = 0, d = 0;
        int num;
        int r = sscanf(str.c_str(),"%u.%u.%u.%u:%u@%u.%u.%u.%u%n",&w,&x,&y,&z,&p,&a,&b,&c,&d,&num);
        if((9 != r) || ((int)str.size() != num) || (w > 255) || (x > 255) || (y > 255) || (z > 255) || (p > 65535) || (a > 255) || (b > 255) || (c > 255) || (d > 255)){
            GUILog(std::string("Invalid multicast configuration \"") + str + std::string("\"!"), 255, 0, 0);
            continue;
        }
        network.multicast.push_back(multicast_configuration());
        network.multicast.back().ipGroup[0] = (uint8_t)w;
        network.multicast.back().ipGroup[1] = (uint8_t)x;
        network.multicast.back().ipGroup[2] = (uint8_t)y;
        network.multicast.back().ipGroup[3] = (uint8_t)z;
        network.multicast.back().port = (uint16_t)p;
        network.multicast.back().ipInterface[0] = (uint8_t)a;
        network.multicast.back().ipInterface[1] = (uint8_t)b;
        network.multicast.back().ipInterface[2] = (uint8_t)c;
        network.multicast.back().ipInterface[3] = (uint8_t)d;
    }
    for(auto&& str : networkJoystick){
        unsigned j = 0, w = 0, x = 0, y = 0, z = 0, p = 0, a = 0, b = 0, c = 0, d = 0;
        int num;
        int r = sscanf(str.c_str(),"%u>%u.%u.%u.%u:%u@%u.%u.%u.%u%n",&j,&w,&x,&y,&z,&p,&a,&b,&c,&d,&num);
        if((10 != r) || ((int)str.size() != num) || (j > 255) || (w > 255) || (x > 255) || (y > 255) || (z > 255) || (p > 65535) || (a > 255) || (b > 255) || (c > 255) || (d > 255)){
            GUILog(std::string("Invalid joystick configuration \"") + str + std::string("\"!"), 255, 0, 0);
            continue;
        }
        network.joystick.push_back(joystick_configuration());
        network.joystick.back().id = (uint8_t)j;
        network.joystick.back().ipDestination[0] = (uint8_t)w;
        network.joystick.back().ipDestination[1] = (uint8_t)x;
        network.joystick.back().ipDestination[2] = (uint8_t)y;
        network.joystick.back().ipDestination[3] = (uint8_t)z;
        network.joystick.back().port = (uint16_t)p;
        network.joystick.back().ipInterface[0] = (uint8_t)a;
        network.joystick.back().ipInterface[1] = (uint8_t)b;
        network.joystick.back().ipInterface[2] = (uint8_t)c;
        network.joystick.back().ipInterface[3] = (uint8_t)d;
    }

    // deserialize initialDB
    for(auto&& str : initialDB){
        // First scan for IP:PORT@INTERFACE
        unsigned w = 0, x = 0, y = 0, z = 0, p = 0, a = 0, b = 0, c = 0, d = 0;
        int num;
        int r = sscanf(str.c_str(),"%u.%u.%u.%u:%u@%u.%u.%u.%u%n",&w,&x,&y,&z,&p,&a,&b,&c,&d,&num);
        if((9 != r) || (w > 255) || (x > 255) || (y > 255) || (z > 255) || (p > 65535) || (a > 255) || (b > 255) || (c > 255) || (d > 255)){
            GUILog(std::string("Invalid network settings for initial database configuration \"") + str + std::string("\"!"), 255, 0, 0);
            continue;
        }

        // Get remaining characters of line (line now starts with ',' after INTERFACE)
        std::string line = str.substr(num);

        // Scan dimension and offset
        float dx, dy, dz, ox, oy, oz;
        r = sscanf(line.c_str(),",%f,%f,%f,%f,%f,%f,%n",&dx,&dy,&dz,&ox,&oy,&oz,&num);
        if(6 != r){
            GUILog(std::string("Invalid dimension/offset settings for initial database configuration \"") + str + std::string("\"!"), 255, 0, 0);
            continue;
        }

        // Get remaining characters of line (line now starts with '"' of first string)
        line = line.substr(num);

        // Get name and description
        if(!line.size() || ('"' != line[0])){
            GUILog(std::string("Invalid vehicle name/description for initial database configuration \"") + str + std::string("\"!"), 255, 0, 0);
            continue;
        }
        auto se = line.find_first_of('"',1);
        if(se == std::string::npos){
            GUILog(std::string("Invalid vehicle name/description for initial database configuration \"") + str + std::string("\"!"), 255, 0, 0);
            continue;
        }
        std::string name = line.substr(1,se - 1);
        line = line.substr(se + 1);
        if((line.size() < 3) || (',' != line[0]) || ('"' != line[1]) || ('"' != line.back())){
            GUILog(std::string("Invalid vehicle name/description for initial database configuration \"") + str + std::string("\"!"), 255, 0, 0);
            continue;
        }
        std::string description = line.substr(2, line.size() - 3);

        // Decoding succeeded, add new entry
        initialDatabase.push_back(database_entry_configuration());
        initialDatabase.back().ipAddress[0] = (uint8_t)w;
        initialDatabase.back().ipAddress[1] = (uint8_t)x;
        initialDatabase.back().ipAddress[2] = (uint8_t)y;
        initialDatabase.back().ipAddress[3] = (uint8_t)z;
        initialDatabase.back().port = (uint16_t)p;
        initialDatabase.back().ipInterface[0] = (uint8_t)a;
        initialDatabase.back().ipInterface[1] = (uint8_t)b;
        initialDatabase.back().ipInterface[2] = (uint8_t)c;
        initialDatabase.back().ipInterface[3] = (uint8_t)d;
        initialDatabase.back().dimension[0] = dx;
        initialDatabase.back().dimension[1] = dy;
        initialDatabase.back().dimension[2] = dz;
        initialDatabase.back().offset[0] = ox;
        initialDatabase.back().offset[1] = oy;
        initialDatabase.back().offset[2] = oz;
        initialDatabase.back().name = name;
        initialDatabase.back().description = description;
    }

    // check for valid values
    if((view.geographicOrigin.latitude < -1.570796326794896615) || (view.geographicOrigin.latitude > 1.570796326794896615)){
        GUILog(std::string("Invalid value for ui.view.geographicOrigin.latitude in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        view.geographicOrigin.latitude = 0.0;
    }
    if((view.geographicOrigin.longitude < -3.14159265358979323) || (view.geographicOrigin.longitude > 3.14159265358979323)){
        GUILog(std::string("Invalid value for ui.view.geographicOrigin.longitude in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        view.geographicOrigin.longitude = 0.0;
    }
    if(view.camera.clipNear <= 0.0){
        GUILog(std::string("Invalid value for ui.view.camera.clipNear in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        view.camera.clipNear = CAMERA_CLIP_NEAR_DEFAULT;
    }
    if(view.camera.clipFar <= 0.0){
        GUILog(std::string("Invalid value for ui.view.camera.clipFar in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        view.camera.clipFar = CAMERA_CLIP_FAR_DEFAULT;
    }
    if((view.display.gamma < GAMMA_MIN) || (view.display.gamma > GAMMA_MAX)){
        GUILog(std::string("Invalid value for ui.view.display.gamma in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        view.display.gamma = GAMMA_DEFAULT;
    }
    if((scene.ambient < 0.0) || (scene.ambient > 1.0)){
        GUILog(std::string("Invalid value for ui.scene.ambient in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        scene.ambient = 0.1;
    }
    if(scene.specularity <= 0.0){
        GUILog(std::string("Invalid value for ui.scene.specularity in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        scene.specularity = 1.0;
    }
    if(scene.shininess <= 0.0){
        GUILog(std::string("Invalid value for ui.scene.shininess in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        scene.shininess = 32.0;
    }
    if((scene.reflection < 0.0) || (scene.reflection > 1.0)){
        GUILog(std::string("Invalid value for ui.scene.reflection in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        scene.reflection = 0.1;
    }
    if(defaultVehicleStyle.positionHistory.lineWidth < 0.0){
        GUILog(std::string("Invalid value for ui.defaultVehicleStyle.positionHistory.lineWidth in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        defaultVehicleStyle.positionHistory.lineWidth = 2.0;
    }
    if(defaultVehicleStyle.waypoints.lineWidth < 0.0){
        GUILog(std::string("Invalid value for ui.defaultVehicleStyle.waypoints.lineWidth in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        defaultVehicleStyle.waypoints.lineWidth = 2.0;
    }
    if(defaultVehicleStyle.trajectory.lineWidth < 0.0){
        GUILog(std::string("Invalid value for ui.defaultVehicleStyle.trajectory.lineWidth in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        defaultVehicleStyle.trajectory.lineWidth = 2.0;
    }
    if(defaultVehicleStyle.compass.radius < 0.0){
        GUILog(std::string("Invalid value for ui.defaultVehicleStyle.compass.radius in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        defaultVehicleStyle.compass.radius = 10.0;
    }
    if((defaultVehicleStyle.waypoints.vehicleAlpha < 0.0) || (defaultVehicleStyle.waypoints.vehicleAlpha > 1.0)){
        GUILog(std::string("Invalid value for ui.defaultVehicleStyle.waypoints.vehicleAlpha in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        defaultVehicleStyle.waypoints.vehicleAlpha = 0.5;
    }
    if((defaultVehicleStyle.trajectory.vehicleAlpha < 0.0) || (defaultVehicleStyle.trajectory.vehicleAlpha > 1.0)){
        GUILog(std::string("Invalid value for ui.defaultVehicleStyle.trajectory.vehicleAlpha in configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
        defaultVehicleStyle.trajectory.vehicleAlpha = 0.5;
    }
}

void GCSConfiguration::Save(void){
    // create json structure
    nlohmann::json jsonData;
    jsonData["window"]["width"]                                                         = window.width;
    jsonData["window"]["height"]                                                        = window.height;
    jsonData["window"]["fullscreen"]                                                    = window.fullscreen;
    jsonData["window"]["fullscreenMonitorIndex"]                                        = window.fullscreenMonitorIndex;
    jsonData["window"]["resizable"]                                                     = window.resizable;
    jsonData["window"]["visible"]                                                       = window.visible;
    jsonData["window"]["decorated"]                                                     = window.decorated;
    jsonData["window"]["focused"]                                                       = window.focused;
    jsonData["window"]["maximized"]                                                     = window.maximized;
    jsonData["window"]["glMajorVersion"]                                                = window.glMajorVersion;
    jsonData["window"]["glMinorVersion"]                                                = window.glMinorVersion;
    jsonData["ui"]["textToSpeech"]["enable"]                                            = textToSpeech.enable;
    jsonData["ui"]["textToSpeech"]["windows"]["systemPreString"]                        = textToSpeech.windows.systemPreString;
    jsonData["ui"]["textToSpeech"]["windows"]["systemPostString"]                       = textToSpeech.windows.systemPostString;
    jsonData["ui"]["textToSpeech"]["linux"]["systemPreString"]                          = textToSpeech.linuxOS.systemPreString;
    jsonData["ui"]["textToSpeech"]["linux"]["systemPostString"]                         = textToSpeech.linuxOS.systemPostString;
    jsonData["ui"]["view"]["geographicOrigin"]["useFromFirstReceivedNavigationMessage"] = view.geographicOrigin.useFromFirstReceivedNavigationMessage;
    jsonData["ui"]["view"]["geographicOrigin"]["latitude"]                              = view.geographicOrigin.latitude / 0.0174532925199433;
    jsonData["ui"]["view"]["geographicOrigin"]["longitude"]                             = view.geographicOrigin.longitude / 0.0174532925199433;
    jsonData["ui"]["view"]["geographicOrigin"]["altitude"]                              = view.geographicOrigin.altitude;
    jsonData["ui"]["view"]["camera"]["enable3D"]                                        = view.camera.enable3D;
    jsonData["ui"]["view"]["camera"]["clipNear"]                                        = view.camera.clipNear;
    jsonData["ui"]["view"]["camera"]["clipFar"]                                         = view.camera.clipFar;
    jsonData["ui"]["view"]["display"]["gamma"]                                          = view.display.gamma;
    jsonData["ui"]["view"]["display"]["groundColor"]                                    = view.display.groundColor;
    jsonData["ui"]["scene"]["ambient"]                                                  = scene.ambient;
    jsonData["ui"]["scene"]["specularity"]                                              = scene.specularity;
    jsonData["ui"]["scene"]["shininess"]                                                = scene.shininess;
    jsonData["ui"]["scene"]["reflection"]                                               = scene.reflection;
    jsonData["ui"]["defaultVehicleStyle"]["altitudeToZero"]                             = defaultVehicleStyle.altitudeToZero;
    jsonData["ui"]["defaultVehicleStyle"]["positionHistory"]["enable"]                  = defaultVehicleStyle.positionHistory.enable;
    jsonData["ui"]["defaultVehicleStyle"]["positionHistory"]["maximumSOG"]              = defaultVehicleStyle.positionHistory.maximumSOG;
    jsonData["ui"]["defaultVehicleStyle"]["positionHistory"]["lineWidth"]               = defaultVehicleStyle.positionHistory.lineWidth;
    jsonData["ui"]["defaultVehicleStyle"]["positionHistory"]["updatePeriodMs"]          = defaultVehicleStyle.positionHistory.updatePeriodMs;
    jsonData["ui"]["defaultVehicleStyle"]["positionHistory"]["bufferSize"]              = defaultVehicleStyle.positionHistory.bufferSize;
    jsonData["ui"]["defaultVehicleStyle"]["waypoints"]["enable"]                        = defaultVehicleStyle.waypoints.enable;
    jsonData["ui"]["defaultVehicleStyle"]["waypoints"]["color"]                         = defaultVehicleStyle.waypoints.color;
    jsonData["ui"]["defaultVehicleStyle"]["waypoints"]["vehicleColor"]                  = defaultVehicleStyle.waypoints.vehicleColor;
    jsonData["ui"]["defaultVehicleStyle"]["waypoints"]["vehicleAlpha"]                  = defaultVehicleStyle.waypoints.vehicleAlpha;
    jsonData["ui"]["defaultVehicleStyle"]["waypoints"]["lineWidth"]                     = defaultVehicleStyle.waypoints.lineWidth;
    jsonData["ui"]["defaultVehicleStyle"]["waypoints"]["symbolSize"]                    = defaultVehicleStyle.waypoints.symbolSize;
    jsonData["ui"]["defaultVehicleStyle"]["trajectory"]["enable"]                       = defaultVehicleStyle.trajectory.enable;
    jsonData["ui"]["defaultVehicleStyle"]["trajectory"]["color"]                        = defaultVehicleStyle.trajectory.color;
    jsonData["ui"]["defaultVehicleStyle"]["trajectory"]["vehicleColor"]                 = defaultVehicleStyle.trajectory.vehicleColor;
    jsonData["ui"]["defaultVehicleStyle"]["trajectory"]["vehicleAlpha"]                 = defaultVehicleStyle.trajectory.vehicleAlpha;
    jsonData["ui"]["defaultVehicleStyle"]["trajectory"]["vehicleStride"]                = defaultVehicleStyle.trajectory.vehicleStride;
    jsonData["ui"]["defaultVehicleStyle"]["trajectory"]["lineWidth"]                    = defaultVehicleStyle.trajectory.lineWidth;
    jsonData["ui"]["defaultVehicleStyle"]["compass"]["enable"]                          = defaultVehicleStyle.compass.enable;
    jsonData["ui"]["defaultVehicleStyle"]["compass"]["radius"]                          = defaultVehicleStyle.compass.radius;
    jsonData["ui"]["defaultVehicleStyle"]["compass"]["color"]                           = defaultVehicleStyle.compass.color;
    jsonData["ui"]["defaultVehicleStyle"]["polygons"]["enable"]                         = defaultVehicleStyle.polygons.enable;
    jsonData["ui"]["defaultVehicleStyle"]["polygons"]["upperLimit"]                     = defaultVehicleStyle.polygons.upperLimit;
    jsonData["ui"]["defaultVehicleStyle"]["polygons"]["lowerLimit"]                     = defaultVehicleStyle.polygons.lowerLimit;
    jsonData["ui"]["defaultVehicleStyle"]["polygons"]["color"]                          = defaultVehicleStyle.polygons.color;
    std::vector<std::string> strUnicast;
    char* buf = new char[1024];
    for(auto&& unicast : network.unicast){
        snprintf(buf, 1024, "%u@%u.%u.%u.%u", unicast.port, unicast.ipInterface[0], unicast.ipInterface[1], unicast.ipInterface[2], unicast.ipInterface[3]);
        strUnicast.push_back(buf);
    }
    jsonData["network"]["unicast"] = strUnicast;
    std::vector<std::string> strMulticast;
    for(auto&& multicast : network.multicast){
        snprintf(buf, 1024, "%u.%u.%u.%u:%u@%u.%u.%u.%u", multicast.ipGroup[0], multicast.ipGroup[1], multicast.ipGroup[2], multicast.ipGroup[3], multicast.port, multicast.ipInterface[0], multicast.ipInterface[1], multicast.ipInterface[2], multicast.ipInterface[3]);
        strMulticast.push_back(buf);
    }
    jsonData["network"]["multicast"] = strMulticast;
    std::vector<std::string> strJoystick;
    for(auto&& joystick : network.joystick){
        snprintf(buf, 1024, "%u>%u.%u.%u.%u:%u@%u.%u.%u.%u", joystick.id, joystick.ipDestination[0], joystick.ipDestination[1], joystick.ipDestination[2], joystick.ipDestination[3], joystick.port, joystick.ipInterface[0], joystick.ipInterface[1], joystick.ipInterface[2], joystick.ipInterface[3]);
        strJoystick.push_back(buf);
    }
    jsonData["network"]["joystick"] = strJoystick;
    std::vector<std::string> strDatabase;
    for(auto&& e : initialDatabase){
        snprintf(buf, 1024, "%u.%u.%u.%u:%u@%u.%u.%u.%u,%f,%f,%f,%f,%f,%f", e.ipAddress[0], e.ipAddress[1], e.ipAddress[2], e.ipAddress[3], e.port, e.ipInterface[0], e.ipInterface[1], e.ipInterface[2], e.ipInterface[3], e.dimension[0], e.dimension[1], e.dimension[2], e.offset[0], e.offset[1], e.offset[2]);
        strDatabase.push_back(std::string(buf) + std::string(",\"") + e.name + std::string("\",\"") + e.description + std::string("\""));
    }
    jsonData["initialDatabase"] = strDatabase;
    delete[] buf;

    // write to file
    std::string filename = FileManager::GetName(FILE_CONFIGURATION_GCS);
    try{
        std::ofstream outstream(filename);
        outstream << std::setw(4) << jsonData << std::endl;
    }
    catch(...){
        GUILog(std::string("Could not save configuration file \"") + filename + std::string("\"!"), 255, 0, 0);
    }
}

