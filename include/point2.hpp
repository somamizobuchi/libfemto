#pragma once

#include <tuple>
#include <istream>
#include <ostream>

namespace femto
{
    /// @brief 2D point2_t class
    /// @tparam T the underlying type
    template <typename T>
    class point2_t
    {
    private:
        T m_x;
        T m_y;

    public:
        /// @brief Constructor for 2D point
        /// @param x X
        /// @param y Y
        point2_t(T x = T{}, T y = T{}) : m_x(x), m_y(y)
        {
        }

        ~point2_t() = default;

        /// @brief Get the X and Y value as a std::pair
        /// @return std::pair <x, y>
        std::pair<T, T> get_pair()
        {
            return std::pair<T, T>(m_x, m_y);
        }

        /// @brief Getter for X
        /// @return X value
        T x() const
        {
            return m_x;
        }

        /// @brief Getter for Y
        /// @return Y value
        T y() const
        {
            return m_y;
        }

        // Addition operator
        point2_t operator+(const point2_t &other) const
        {
            return point2_t(m_x + other.x(), m_y + other.y());
        }

        // Subtraction operator
        point2_t operator-(const point2_t &other) const
        {
            return point2_t(m_x - other.x(), m_y - other.y());
        }

        // Scalar multiplication
        point2_t operator*(const T &scalar) const
        {
            return point2_t(m_x * scalar, m_y * scalar);
        }

        // Dot product
        T operator*(const point2_t &other) const
        {
            return m_x * other.x() + m_y * other.y();
        }

        // Compound addition
        point2_t &operator+=(const point2_t &other)
        {
            m_x += other.x();
            m_y += other.y();
            return *this;
        }

        // Equality operator
        bool operator==(const point2_t &other) const
        {
            return m_x == other.x() && m_y == other.y();
        }

        // Inequality operator
        bool operator!=(const point2_t &other) const
        {
            return !(*this == other);
        }

        // Magnitude (length) of vector
        T magnitude() const
        {
            return std::sqrt(m_x * m_x + m_y * m_y);
        }

        // Stream output operator
        friend std::ostream &operator<<(std::ostream &os, const point2_t &p)
        {
            os << "(" << p.m_x << ", " << p.m_y << ")";
            return os;
        }

        // Stream input operator
        friend std::istream &operator>>(std::istream &is, point2_t &p)
        {
            is >> p.m_x >> p.m_y;
            return is;
        }
    };

    // Non-member scalar multiplication (for scalar * point)
    template <typename T>
    point2_t<T> operator*(const T &scalar, const point2_t<T> &point)
    {
        return point * scalar;
    }

}