#pragma once
#include <cmath>

namespace Bolt {
    template<typename T>
    struct Constants {
        static constexpr T Pi = T(3.14159265358979323846);
        static constexpr T HalfPi = Pi * T(0.5);
        static constexpr T TwoPi = Pi * T(2.0);
        static constexpr T rad2deg = T(180.0) / Pi;
        static constexpr T deg2rad = Pi / T(180.0);
    };

    template<typename T>
    inline T Radians(T degrees) {
        return degrees * Constants<T>::deg2rad;
    }

    template<typename T>
    inline T Degrees(T radians) {
        return radians * Constants<T>::rad2deg;
    }

    template<typename T>
    inline T Sin(T angle) {
        return std::sin(angle);
    }

    template<typename T>
    inline T Cos(T angle) {
        return std::cos(angle);
    }

    template<typename T>
    inline T Tan(T angle) {
        return std::tan(angle);
    }

    // Inverse trigonometrische Funktionen
    template<typename T>
    inline T Asin(T value) {
        return std::asin(value);
    }

    template<typename T>
    inline T Acos(T value) {
        return std::acos(value);
    }

    template<typename T>
    inline T Atan(T value) {
        return std::atan(value);
    }

    template<typename T>
    inline T Atan2(T y, T x) {
        return std::atan2(y, x);
    }

    // Hyperbolische Funktionen (optional)
    template<typename T>
    inline T Sinh(T angle) {
        return std::sinh(angle);
    }

    template<typename T>
    inline T Cosh(T angle) {
        return std::cosh(angle);
    }

    template<typename T>
    inline T Tanh(T angle) {
        return std::tanh(angle);
    }

    // Konstanten-Shortcuts
    template<typename T> constexpr T Pi() { return Constants<T>::Pi; }
    template<typename T> constexpr T HalfPi() { return Constants<T>::HalfPi; }
    template<typename T> constexpr T TwoPi() { return Constants<T>::TwoPi; }

    // Nützliche Hilfsfunktionen
    template<typename T>
    inline void Sincos(T angle, T& sinValue, T& cosValue) {
        sinValue = std::sin(angle);
        cosValue = std::cos(angle);
    }

    // Normalisiert einen Winkel auf den Bereich [0, 2pi]
    template<typename T>
    inline T normalizeAngle(T angle) {
        angle = std::fmod(angle, Constants<T>::TwoPi);
        if (angle < 0) angle += Constants<T>::TwoPi;
        return angle;
    }

    // Normalisiert einen Winkel auf den Bereich [-pi, pi]
    template<typename T>
    inline T normalizeAngleSigned(T angle) {
        angle = normalizeAngle(angle);
        if (angle > Constants<T>::Pi) angle -= Constants<T>::TwoPi;
        return angle;
    }
}