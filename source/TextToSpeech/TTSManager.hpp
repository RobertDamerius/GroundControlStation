#pragma once


class TTSManager {
    public:
        /**
         *  @brief Initialize the text-to-speech manager.
         */
        void Initialize(void);

        /**
         *  @brief Terminate the text-to-speech manager.
         */
        void Terminate(void);

        /**
         *  @brief Add text to the queue.
         *  @param [in] text The text to be spoken.
         */
        void AddToQueue(std::string& text);

    private:
        std::queue<std::string> commandQueue; ///< The queue of system commands.
        std::mutex mtx;                       ///< Mutex to protect @ref commandQueue.
        std::condition_variable cv;           ///< Condition variable for thread notification.
        std::atomic<bool> terminate;          ///< True if @ref ConsumerThread should be terminated.
        std::thread t;                        ///< Consumer thread object.

        void ConsumerThread(void);
};

