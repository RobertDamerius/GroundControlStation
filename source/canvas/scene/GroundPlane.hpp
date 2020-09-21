#pragma once


class GroundPlane {
    public:
        glm::vec3 color; ///< Linear color of ground plane (reverse gamma corrected: rgb*rgb).

        /**
         *  @brief Create a ground plane.
         */
        GroundPlane();

        /**
         *  @brief Delete the ground plane.
         */
        ~GroundPlane();

        /**
         *  @brief Draw the ground plane.
         */
        void Draw(void);

        /**
         *  @brief Reset to default states.
         */
        void Reset(void);
};

