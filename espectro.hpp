#pragma once

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "complex.hpp"
#include "FFT.hpp"

const std::filesystem::path EXPORT_DIR = "export";

void clear_export_type(const std::string& type) {
    std::filesystem::create_directories(EXPORT_DIR);

    for (const auto& entry : std::filesystem::directory_iterator(EXPORT_DIR)) {
        if (!entry.is_regular_file()) continue;

        const std::string name = entry.path().filename().string();
        const bool same_type = name.rfind(type + "_", 0) == 0;
        const bool is_csv = entry.path().extension() == ".csv";

        if (same_type && is_csv) {
            std::filesystem::remove(entry.path());
        }
    }
}

void export_spectrum_csv(
    const char* filename,
    const std::vector<Complex>& X,
    double fs
){
    std::filesystem::create_directories(EXPORT_DIR);

    const std::filesystem::path path = EXPORT_DIR / filename;
    std::ofstream file(path);

    if (!file) {
        std::cerr << "No se pudo crear el archivo " << path << "\n";
        return;
    }

    int N = (int)X.size();

    file << "k,freq,re,im,mag,phase\n";

    for (int k = 0; k < N; k++){
        double freq = k * fs / N;

        file << k << ","
            << freq << ","
            << X[k].re << ","
            << X[k].im << ","
            << X[k].magnitude() << ","
            << X[k].get_angle() << "\n"; // Fase
    }
}
