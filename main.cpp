#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "complex.hpp"
#include "FFT.hpp"
#include "espectro.hpp"
#include "audio.hpp"

// Cantidad de pedazos en los que parto el audio.
const int BLOCK_COUNT = 4;


// Guardo el bloque ya pasado a Fourier, pero tambien guardo donde iba en el audio original.
// Esto es importante porque despues de hacer la inversa tengo que volver a pegar cada pedazo
// en su lugar, no simplemente tirarlos todos a lo loco. (lo aprendi a la mala xdd)
struct FourierBlock {
    std::vector<Complex> spectrum;
    size_t start;
    size_t length;
};

int main() {
    std::cout << "Hola loko, como se llama tu archivo?: " << std::endl;
    std::string filename;
    std::cin >> filename;

    std::cout << "Procesando" << std::endl;

    // Cargo el wav y lo dejo mono. Si el archivo tenia varios canales, audio.hpp los promedia.
    WavData data = load_wav_mono(filename.c_str());

    std::cout << "Audio data: " << data.sampleRate << ", "
              << data.channels << ", " << data.frames << std::endl;

    if (data.frames == 0) {
        std::cerr << "El archivo no tiene frames de audio." << std::endl;
        return 1;
    }

    const size_t total_frames = static_cast<size_t>(data.frames);
    const size_t actual_blocks = std::min(static_cast<size_t>(BLOCK_COUNT), total_frames);

    // Primero limpio los Fourier viejos para que no se mezclen resultados de otra corrida.
    // clear_export_type("fourier") borra export/fourier_*.csv y deja quietos los otros tipos.
    clear_export_type("fourier");
    std::vector<FourierBlock> blocks;

    // Dividir todo el audio en N bloques.
    for (size_t block = 0; block < actual_blocks; block++) {
        const size_t start = block * total_frames / actual_blocks;
        const size_t end = (block + 1) * total_frames / actual_blocks;

        // Armo el vector complejo del bloque: parte real = audio, parte imaginaria = 0.
        // La FFT trabaja con complejos aunque el wav venga como senal real.
        std::vector<Complex> x(end - start);
        for (size_t i = 0; i < x.size(); i++) {
            x[i].re = data.mono[start + i];
            x[i].im = 0.0;
        }

        // Mi FFT recursiva necesita tamano potencia de 2.
        // pad rellena con Complex(0, 0), o sea silencio, hasta la siguiente potencia.
        dft::pad(x);

        // invert = false => transformada normal, pasamos de tiempo a frecuencia.
        dft::fft(x, false);

        // Me guardo el espectro para poder hacer la inversa despues sin leer el CSV.
        blocks.push_back({x, start, end - start}); 

        // Exporto un CSV por bloque. Esto sirve para mirar magnitud, fase, etc.
        std::string name = "fourier_" + std::to_string(block) + ".csv";
        export_spectrum_csv(name.c_str(), x, data.sampleRate);
        std::cout << "Exportado " << name << " con frames [" << start << ", " << end << ")" << std::endl;
    }
	
    // Ahora hacemos el camino de vuelta: Fourier -> senal normal.
    // Limpio solo los normal_*.csv viejos antes de escribir los nuevos.
    clear_export_type("normal");

    // Aca voy sumando los bloques reconstruidos.
    std::vector<float> reconstructed(total_frames, 0.0f);

    for (size_t block = 0; block < blocks.size(); block++) {
		
        std::vector<Complex>& bloque = blocks[block].spectrum;

        // invert = true => inversa de Fourier, volvemos de frecuencia a tiempo.
        dft::fft(bloque, true);

        // Solo copio length muestras reales.
        // El vector puede ser mas largo por el padding, y ese padding no pertenece al audio original.
        for (size_t i = 0; i < blocks[block].length; i++) {
            reconstructed[blocks[block].start + i] += static_cast<float>(bloque[i].re);
        }

        // Exporto el bloque reconstruido tambien, para comparar con fourier_*.csv si hace falta.
        std::string name = "normal_" + std::to_string(block) + ".csv";
        export_spectrum_csv(name.c_str(), bloque, data.sampleRate);
        std::cout << "Exportado " << name << std::endl;
    }

    // Escribo el wav reconstruido final..
    const std::string reconstructed_name = (EXPORT_DIR / "reconstructed.wav").string();
    if (save_wav_mono(reconstructed_name.c_str(), reconstructed, data.sampleRate)) {
        std::cout << "Exportado " << reconstructed_name << std::endl;
    }

    // Llamo el script de matplotlib desde C++ para dejar listas las imagenes. (chagpt)
    // --no-show hace que guarde los PNG sin abrir una ventana encima.
    const int plot_status = std::system("python plot_spectrograms.py");
    if (plot_status != 0) {
        std::cerr << "No se pudo correr plot_spectrograms.py automaticamente. "
                  << "Puedes probar manualmente con: python plot_spectrograms.py" << std::endl;
    }

    return 0;
}
