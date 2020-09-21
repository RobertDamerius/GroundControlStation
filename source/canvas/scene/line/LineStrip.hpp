#pragma once


#include <LinePrimitive.hpp>


class LineStrip: public LinePrimitive {
    public:
        /**
         *  @brief Create a line strip.
         */
        LineStrip();

        /**
         *  @brief Default copy constructor.
         */
        LineStrip(const LineStrip& l) = default;

        /**
         *  @brief Delete the line strip.
         */
        ~LineStrip();

        /**
         *  @brief Set the vertices.
         *  @param [in] v Container of vertices. The parameter may be changed by the call.
         */
        void SetVertices(std::vector<LineVertex>& v);

        /* Operators */
        LineStrip& operator=(const LineStrip& rhs);
};

