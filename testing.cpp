#pragma once

#include <vector>
#include "complex.hpp"
#include "FFT.hpp"
#include "espectro.hpp"

const double EPS = 1e-9;
const int n = 32;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& values) {
    os << "[";
    for (size_t i = 0; i < values.size(); i++) {
        if (i > 0) os << ", ";
        os << values[i];
    }
    os << "]";
    return os;
}

bool is_same(const Complex& a, const Complex& b){
    if (std::abs(a.re - b.re) > EPS) return 0;
    if (std::abs(a.im - b.im) > EPS) return 0;
    return 1;
}

int main() {
    // PART 1: DFT (SALE BIEN)
    std::vector<Complex> a(n);
    for (int i = 0; i < n; i++) a[i] = Complex(i, 0);
    std::cout << "ORIGINAL: " << a << std::endl;

    std::vector<Complex> original = a;
    dft::pad(a);

    dft::fft(a, false);
    std::cout << "DFT: " << a << std::endl;

    dft::fft(a, true);
    std::cout << "NORMAL: " << a << std::endl;

    std::vector<bool> same(n);
    bool all_ok = 1;
    for (int i = 0; i < n; i++){
        same[i] = is_same(a[i], original[i]);
        all_ok &= same[i];
    } 

    std::cout << "EQUAL: " << same << std::endl;
    std::cout << all_ok << std::endl;

    // PART 2: ESPECTRO (EXPORTA BIEN :) )
    std::vector<Complex> constante(n, Complex(1, 0));

    dft::pad(constante);
    dft::fft(constante, false);
    std::cout << constante << std::endl;
    export_spectrum_csv("constant.csv", constante, 1.0);


    return 0;
}
