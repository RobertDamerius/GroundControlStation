#pragma once


#include <PolygonObstacle.hpp>
#include <ShaderVehicle.hpp>


class PolygonContainer {
    public:
        GLfloat upperLimit;            ///< Upper limit for the polyhedron.
        GLfloat lowerLimit;            ///< Lower limit for the polyhedron.
        glm::vec3 color;               ///< Total polygon color.
        bool enable;                   ///< True if polygon container should be rendered.
        std::vector<bool> segmentInfo; ///< Segment information (only used by the network manager). True means already received.

        /**
         *  @brief Create a polygon container.
         */
        PolygonContainer();

        /**
         *  @brief Copy constructor.
         */
        PolygonContainer(const PolygonContainer& p) = default;

        /**
         *  @brief Delete the polygon container.
         */
        ~PolygonContainer();

        /**
         *  @brief Create the internal @ref mtx.
         */
        void CreateMutex(void);

        /**
         *  @brief Delete the internal @ref mtx.
         */
        void DeleteMutex(void);

        /**
         *  @brief Render polygons.
         *  @param [in] shader The polygon shader.
         */
        void Render(ShaderVehicle& shader);

        /**
         *  @brief Delete the GL content for all polygons that are still generated.
         */
        void DeleteGL(void);

        /**
         *  @brief Add a set of new polygons to the container.
         *  @param [in] polygons Set of polygons.
         *  @param [in] velocities Set of velocities (u,v,r) for all polygons. Size must be equal to size of polygons.
         *  @details The GL buffer objects are generated during the next rendering call.
         */
        void AddPolygons(std::vector<std::vector<std::array<double, 2>>>& polygons, std::vector<std::array<double, 3>> velocities);

        /**
         *  @brief Clear all polygons.
         *  @details All generated polygons will be moved to the @ref polytopesToDelete container
         *  and the @ref polygons container will be cleared. The GL buffers are then deleted during
         *  the next rendering call or when @ref DeleteGL is called.
         */
        void ClearPolygons(void);

        /* Assignment operator */
        PolygonContainer& operator=(const PolygonContainer& rhs);

    private:
        std::vector<PolygonObstacle> polygons;   ///< List of polygons.
        std::vector<Polytope> polytopesToDelete; ///< List of polytopes that should be deleted.
        std::mutex* mtx;                         ///< Protect @ref polygons and @ref polytopesToDelete.
};

