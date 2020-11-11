#include <TTSManager.hpp>
#include <Configuration.hpp>


void TTSManager::Initialize(void){
    // Make sure that the manager is terminated
    Terminate();

    // Create thread
    t = std::thread(&TTSManager::ConsumerThread, this);
}

void TTSManager::Terminate(void){
    // Set termination flag and notify thread
    terminate = true;
    cv.notify_all();

    // Wait until thread is finished
    if(t.joinable()){
        t.join();
    }

    // Clear queue and reset termination flag
    std::queue<std::string> emptyQueue;
    commandQueue.swap(emptyQueue);
    terminate = false;
}

void TTSManager::AddToQueue(std::string& text){
    // Nothing to say
    if(!text.size()){
        return;
    }

    // Check string for valid characters to prevent command injection
    for(auto&& c : text){
        if(((c >= 'a') && (c <= 'z')) ||
           ((c >= 'A') && (c <= 'Z')) || 
           ((c >= '0') && (c <= '9')) || 
           (c == ',') || (c == '.') || (c == '!') || (c == '?') || (c == ' ')){
            continue;
        }
        return;
    }

    // Create final command
    std::string strCommand = Configuration::tts.systemPreString + text + Configuration::tts.systemPostString;

    // Add to queue
    {
        std::lock_guard<std::mutex> lk(mtx);
        commandQueue.push(strCommand);
    }
    cv.notify_all();
}

void TTSManager::ConsumerThread(void){
    for(;;){
        // Wait for event
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [this]{ return terminate || !commandQueue.empty(); });

        // Run all commands from queue
        while(!commandQueue.empty() && !terminate){
            if(Configuration::tts.enable){
                int result = std::system(commandQueue.front().c_str());
                (void) result;
            }
            commandQueue.pop();
        }

        // Break loop if termination flag is set
        if(terminate){
            break;
        }
    }
}

