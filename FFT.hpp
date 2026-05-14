#pragma once

#include <vector>
#include "complex.hpp"

/*
* https://cp-algorithms.com/algebra/fft.html#discrete-fourier-transform
*/

const double PI = std::acos(-1);

namespace dft{

    // Funcion que expande el vector A para que tenga tamano igual a una potencia de dos.
    int pad(std::vector<Complex>& a){
        int n = a.size();
        int new_n = 1;
        
        for (; new_n < n; new_n <<= 1);

        a.resize(new_n);
        return new_n;
    }

    // Implementacion calcada, aunque entendida, de la pagina.
    void fft(std::vector<Complex>& a, bool invert){
        int n = a.size();
        if (n <= 1) return;

        // Separamos en parte par e impar
        std::vector<Complex> par(n / 2), impar(n / 2);
        for (int i = 0; 2 * i < n; i++){
            par[i] = a[2*i];
            impar[i] = a[2*i+1];
        }

        // Sumdividimos constantemente para lograr O(N log N)
        fft(par, invert);
        fft(impar, invert);

        // El w de la matriz de rotacion
        double ang = 2 * PI / n  * (invert ? -1 : 1); // Si estamos en la inversa rotamos pal otro lado
        Complex w(1, 0), wn = Complex::from_angle(ang); // w es el angulo inicial, wn como cambia por rotacion
        
        for (int i = 0; 2 * i < n; i++) {
            // Tecnica mariposa
            a[i] = par[i] + w * impar[i];
            a[i + n/2] = par[i] - w * impar[i];

            // Cuando invertimos temenos que dividir la cosa final por n. Lo hacemos por 2 una cantidad de Log N veces....
            if (invert) {
                a[i] /= 2;
                a[i + n/2] /= 2;
            }

            // Actualizamos la rotacion
            w *= wn; 
        }
    }
}


