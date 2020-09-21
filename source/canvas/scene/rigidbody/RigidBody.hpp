#pragma once


#include <RigidBodyState.hpp>


class RigidBody: public RigidBodyState {
    public:
        glm::mat4 modelMatrix;  ///< Model matrix (calculated by @ref UpdateModelMatrix).

        /**
         *  @brief Create a rigid body.
         */
        RigidBody();

        /**
         *  @brief Default copy constructor.
         */
        RigidBody(const RigidBody& rb) = default;

        /**
         *  @brief Delete the rigid body.
         */
        ~RigidBody();

        /**
         *  @brief Update the @ref modelMatrix attribute.
         *  @details The @ref modelMatrix will be calculated using derived attributes @ref position and @ref quaternion.
         */
        void UpdateModelMatrix(void);

        /* Operators */
        RigidBody& operator=(const RigidBody& rhs);
        RigidBody& operator=(const RigidBodyState& rhs);
};

