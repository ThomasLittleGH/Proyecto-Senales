#pragma once

#include <ostream>
#include <cmath>

class Complex {
    public:
        double re = 0.0;
        double im = 0.0;

        // Constructors
        Complex() {}
        Complex(double in_re, double in_im){
            re = in_re;
            im = in_im;
        }

        Complex(const  Complex& other) = default;

        // Defino comportamiento con los otros complejos (suma, resta)
        Complex& operator+=(const Complex& other){
            re += other.re;
            im += other.im;
            return *this;
        }
        Complex& operator-=(const Complex& other){
            re -= other.re;
            im -= other.im;
            return *this;
        }
        Complex& operator*=(const Complex& other){
            double new_re = re * other.re - im * other.im;
            double new_im = re * other.im + im * other.re;

            re = new_re;
            im = new_im;

            return *this;
        }

        // Defino comportamiento con constantes
        Complex& operator*=(double k){
            re *= k;
            im *= k;
            return *this;
        }
        Complex& operator/=(double k){
            re /= k;
            im /= k;
            return *this;
        }

        // Defino Conjugado
        Complex conjugate() const {
            return Complex(re, -im);
        }

        // Magnitud y angulo por si los necesito dsp, me falta estudiar xdd
        double magnitude2() const {
            return re * re + im * im;
        }

        double magnitude() const {
            return std::sqrt(magnitude2());
        }

        double get_angle() const {
            return std::atan2(im, re);
        }

        // Creo un complejo a partir de un angulo
        static Complex from_polar(double mag, double theta) {
            return Complex(mag * std::cos(theta), mag * std::sin(theta));
        }

        static Complex from_angle(double theta) {
            return from_polar(1.0, theta);
        }
};

// Conveniencia

inline Complex operator+(Complex a, const Complex& b) {
    a += b;
    return a;
}

inline Complex operator-(Complex a, const Complex& b) {
    a -= b;
    return a;
}

inline Complex operator*(Complex a, const Complex& b) {
    a *= b;
    return a;
}

inline Complex operator*(Complex a, double k) {
    a *= k;
    return a;
}

inline Complex operator*(double k, Complex a) {
    a *= k;
    return a;
}

inline Complex operator/(Complex a, double k) {
    a /= k;
    return a;
}

inline std::ostream& operator<<(std::ostream& os, const Complex& obj)
{
    os << obj.re << " " << obj.im;
    return os;
}