#pragma once


#include <LinePrimitive.hpp>


class Line: public LinePrimitive {
    public:
        /**
         *  @brief Create a line.
         */
        Line();

        /**
         *  @brief Default copy constructor.
         */
        Line(const Line& l) = default;

        /**
         *  @brief Delete the line.
         */
        ~Line();

        /**
         *  @brief Set the vertices.
         *  @param [in] v Container of vertices. The parameter may be changed by the call.
         *  @details Two consecutive vertices indicate a line. If the number of vertices is odd, the last vertex is discarded.
         */
        void SetVertices(std::vector<LineVertex>& v);

        /* Operators */
        Line& operator=(const Line& rhs);
};

