#pragma once


class TTSConfiguration {
    public:
        std::string systemPreString;   ///< Pre-string for system command.
        std::string systemPostString;  ///< Post-string for system command.
        bool enable;                   ///< True if text-to-speech should be enabled, false otherwise, defaults to false.

        /**
         *  @brief Create the origin configuration.
         */
        TTSConfiguration();

        /**
         *  @brief Read the origin configuration file.
         */
        void Read(void);
};

