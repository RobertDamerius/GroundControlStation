#pragma once


#include <Polytope.hpp>
#include <RigidBody.hpp>


class PolygonObstacle: public Polytope {
    public:
        RigidBody navigation;     ///< Navigation data of the polygon obstacle.

        /**
         *  @brief Create a polygon obstacle.
         */
        PolygonObstacle();

        /**
         *  @brief Default copy constructor.
         */
        PolygonObstacle(const PolygonObstacle& p) = default;

        /**
         *  @brief Delete the polygon obstacle.
         */
        ~PolygonObstacle();

        /**
         *  @brief Create a polyhedron based on a planar nonconvex polygon.
         *  @param [in] vertices Vertices of the polygon in the NED frame.
         *  @param [in] yMin Minimum y-value for polyhedron (OpenGL worldspace).
         *  @param [in] yMax Maximum y-value for polyhedron (OpenGL worldspace).
         */
        void CreatePolyhedron(std::vector<std::array<double, 2>>& vertices, GLfloat yMin = -1.0f, GLfloat yMax = 1.0f);

        /**
         *  @brief Generate the GL content for the polygon obstacle.
         *  @details Overwrites the Polytope::GenerateGL() member function but calls it internally.
         */
        void GenerateGL(void);

        /**
         *  @brief Delete the GL content for the polygon obstacle.
         *  @details Overwrites the Polytope::DeleteGL() member function but calls it internally.
         */
        void DeleteGL(void);

        /**
         *  @brief Check if the polygon obstacle has been generated.
         *  @return True if polygon obstacle has been generated, false otherwise.
         */
        inline bool IsGenerated(void){ return this->generated; }

        /* Operators */
        PolygonObstacle& operator=(const PolygonObstacle& rhs);

    private:
        bool generated;           ///< True if generated, false otherwise.
};

