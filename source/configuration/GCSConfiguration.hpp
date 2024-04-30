#pragma once


class GCSConfiguration {
    public:
        struct {
            uint32_t width;                                           ///< Width of the window in pixels.
            uint32_t height;                                          ///< Height of the window in pixels.
            bool fullscreen;                                          ///< True if fullscreen should be enabled, false otherwise.
            uint32_t fullscreenMonitorIndex;                          ///< The index of the monitor to be used in fullscreen mode.
            bool resizable;                                           ///< True if window should be resizable, false otherwise.
            bool visible;                                             ///< True if the window should be visible, false otherwise.
            bool decorated;                                           ///< True if the window should be decorated, false otherwise.
            bool focused;                                             ///< True if the window should be focused, false otherwise.
            bool maximized;                                           ///< True if the window should be maximized, false otherwise.
            uint32_t glMajorVersion;                                  ///< The Open-GL major version to be set for the window context.
            uint32_t glMinorVersion;                                  ///< The Open-GL minor version to be set for the window context.
        } window;
        struct {
            bool enable;                                              ///< True if text-to-speech should be enabled, false otherwise.
            struct {
                std::string systemPreString;                          ///< Pre-string for a windows system command.
                std::string systemPostString;                         ///< Post-string for a windows system command.
            } windows;
            struct {
                std::string systemPreString;                          ///< Pre-string for a linux system command.
                std::string systemPostString;                         ///< Post-string for a linux system command.
            } linuxOS;
        } textToSpeech;
        struct {
            struct {
                bool useFromFirstReceivedNavigationMessage;           ///< True if origin is undefined and should be set from first received navigation message.
                double latitude;                                      ///< Latitude in radians.
                double longitude;                                     ///< Longitude in radians.
                double altitude;                                      ///< Altitude in meters.
            } geographicOrigin;
            struct {
                bool enable3D;                                        ///< True if 3D view is to be enabled, false otherwise (2D mode).
                double clipNear;                                      ///< Near clipping plane of view camera.
                double clipFar;                                       ///< Far clipping plane of view camera.
            } camera;
            struct {
                double gamma;                                         ///< Gamma value.
                std::array<uint8_t,3> groundColor;                    ///< Color of the ground plane.
            } display;
        } view;
        struct {
            double ambient;                                           ///< Global ambient strength for light calculation.
            double specularity;                                       ///< Global specularity value for light calculation.
            double shininess;                                         ///< Global shininess value for light calculation.
            double reflection;                                        ///< Global reflection factor for objects.
        } scene;
        struct {
            bool altitudeToZero;                                      ///< True if altitude should be set to zero.
            struct {
                bool enable;                                          ///< True if position history should be enabled, false otherwise.
                double maximumSOG;                                    ///< The color of the position history is adapted to the speed over ground (SOG).
                double lineWidth;                                     ///< Width of the position history line.
                uint32_t updatePeriodMs;                              ///< Time period with which positions are to be added to the position history.
                uint32_t bufferSize;                                  ///< Maximum number of values stored in the position history. If the buffer size is reduced, the oldest positions are deleted.
            } positionHistory;
            struct {
                bool enable;                                          ///< True if waypoints should be enabled, false otherwise.
                std::array<uint8_t,3> color;                          ///< Color of waypoints.
                std::array<uint8_t,3> vehicleColor;                   ///< Color multiplier for the vehicle polyhedron at a waypoint.
                double vehicleAlpha;                                  ///< Normalized alpha value for vehicle polyhedra at waypoint.
                double lineWidth;                                     ///< Width of the line.
                double symbolSize;                                    ///< Size of the waypoint symbol. All waypoints are connected by lines.
            } waypoints;
            struct {
                bool enable;                                          ///< True if trajectory should be enabled, false otherwise.
                std::array<uint8_t,3> color;                          ///< Color of the trajectory.
                std::array<uint8_t,3> vehicleColor;                   ///< Color multiplier for the vehicle polyhedra.
                double vehicleAlpha;                                  ///< Normalized alpha value for vehicle polyhedra.
                double vehicleStride;                                 ///< Specifies the time interval at which the vehicle polyhedron should be displayed along the trajectory.
                double lineWidth;                                     ///< Width of the trajectory line.
            } trajectory;
            struct {
                bool enable;                                          ///< True if compass should be enabled, false otherwise.
                double radius;                                        ///< The radius of the compass in meters.
                std::array<uint8_t,3> color;                          ///< Color of the compass.
            } compass;
            struct {
                bool enable;                                          ///< True if polygons should be enabled, false otherwise.
                double upperLimit;                                    ///< Upper limit (z-value) of the polyhedra.
                double lowerLimit;                                    ///< Lower limit (z-value) of the polyhedra.
                std::array<uint8_t,3> color;                          ///< Color of convex polygons.
            } polygons;
        } defaultVehicleStyle;
        struct unicast_configuration {
            uint16_t port;
            uint8_t ipInterface[4];
        };
        struct multicast_configuration {
            uint8_t ipGroup[4];
            uint16_t port;
            uint8_t ipInterface[4];
        };
        struct joystick_configuration {
            uint8_t id;
            uint8_t ipDestination[4];
            uint16_t port;
            uint8_t ipInterface[4];
        };
        struct {
            std::vector<unicast_configuration> unicast;               ///< JSON string: "PORT@INTERFACE"
            std::vector<multicast_configuration> multicast;           ///< JSON string: "GROUP:PORT@INTERFACE"
            std::vector<joystick_configuration> joystick;             ///< JSON string: "JOYSTICK_ID>DESTINATION_IP:DESTINATION_PORT@INTERFACE"
        } network;
        struct database_entry_configuration {
            uint8_t ipAddress[4];
            uint16_t port;
            uint8_t ipInterface[4];
            std::array<float,3> dimension;
            std::array<float,3> offset;
            std::string name;
            std::string description;
        };
        std::vector<database_entry_configuration> initialDatabase;    ///< JSON string: "IP:PORT@INTERFACE,dx,dy,dz,ox,oy,oz,NAME,DESCRIPTION"


        /**
         * @brief Construct a new GCS configuration and set default values.
         */
        GCSConfiguration();

        /**
         * @brief Clear the configuration and set default values.
         */
        void Clear(void);

        /**
         * @brief Read the configuration from a file.
         */
        void Read(void);

        /**
         * @brief Save the configuration to a file.
         */
        void Save(void);
};

