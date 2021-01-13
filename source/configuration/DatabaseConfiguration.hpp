#pragma once


class DatabaseConfiguration {
    public:
        struct database_entry_configuration {
            uint8_t ipAddress[4];
            uint16_t port;
            uint8_t ipInterface[4];
            std::array<float,3> dimension;
            std::array<float,3> offset;
            std::string name;
            std::string description;
        };
        std::vector<database_entry_configuration> entry;

        /**
         *  @brief Read the network configuration file.
         */
        void Read(void);
};

