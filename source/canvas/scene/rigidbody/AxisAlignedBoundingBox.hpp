#pragma once


class AxisAlignedBoundingBox {
    public:
        glm::dvec3 position;   ///< Position of lowest corner.
        glm::dvec3 dimension;  ///< Dimension for x-, y- and z-axis.

        /**
         *  @brief Create an axis-aligned bounding box.
         *  @param [in] p Position of lowest corner, defaults to glm::dvec3(0.0).
         *  @param [in] d Dimension for x-, y- and z-axis, defaults to glm::dvec3(0.0).
         */
        explicit AxisAlignedBoundingBox(glm::dvec3 p = glm::dvec3(0.0), glm::dvec3 d = glm::dvec3(0.0));

        /**
         *  @brief Delete the axis-aligned bounding box.
         */
        ~AxisAlignedBoundingBox();

        /* Operators */
        AxisAlignedBoundingBox operator+(const AxisAlignedBoundingBox& rhs);
        AxisAlignedBoundingBox operator+=(const AxisAlignedBoundingBox& rhs);
};


AxisAlignedBoundingBox operator*(glm::mat4& lhs, AxisAlignedBoundingBox& rhs);

