#pragma once


class Angle {
    public:
        /**
         *  @brief Create an angle instance.
         *  @param [in] value Initial value, defaults to zero.
         *  @details The default value will be zero.
         */
        Angle(double value = 0.0);

        /**
         *  @brief Copy constructor.
         */
        Angle(const Angle& a) = default;

        /**
         *  @brief Delete the angle instance.
         */
        ~Angle();

        /**
         *  @brief Get angle in degrees.
         *  @return Angle in degrees.
         */
        double Deg(void);

        /**
         *  @brief Get positive angle in degrees.
         *  @return Positive angle, range [0, 2*pi), in dregrees.
         */
        double PositiveDeg(void);

        /* Operators */
        Angle& operator=(const Angle& rhs);
        Angle& operator=(const double& rhs);
        double operator()(void);
        Angle operator+(const Angle& rhs);
        Angle operator+(const double& rhs);
        Angle operator-(const Angle& rhs);
        Angle operator-(const double& rhs);
        Angle operator-(void);
        Angle operator*(const double& rhs);
        Angle operator/(const double& rhs);
        Angle& operator+=(const Angle& rhs);
        Angle& operator+=(const double& rhs);
        Angle& operator-=(const Angle& rhs);
        Angle& operator-=(const double& rhs);
        Angle& operator*=(const Angle& rhs);
        Angle& operator*=(const double& rhs);
        Angle& operator/=(const Angle& rhs);
        Angle& operator/=(const double& rhs);

        friend Angle operator+(double, const Angle&);
        friend Angle operator-(double, const Angle&);
        friend Angle operator*(double, const Angle&);
        friend Angle operator/(double, const Angle&);

    private:
        double value;   ///< Internal angle value in radians, range is [-pi, +pi).

        /**
         *  @brief Convert to symmetrical range.
         *  @param [in] d Input value.
         *  @return Symmetrical value.
         */
        static double SymmetricalRange(double d);
};


Angle operator+(double lvalue, const Angle& rvalue);
Angle operator-(double lvalue, const Angle& rvalue);
Angle operator*(double lvalue, const Angle& rvalue);
Angle operator/(double lvalue, const Angle& rvalue);

