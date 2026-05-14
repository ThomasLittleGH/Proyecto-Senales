#pragma once

#define DR_WAV_IMPLEMENTATION

#include <filesystem>
#include <iostream>
#include <vector>
#include "complex.hpp"
#include "FFT.hpp"
#include "dr_wav.h"

struct WavData {
    unsigned int sampleRate;
    unsigned int channels;
    unsigned long long frames;
    std::vector<float> mono;
};

WavData load_wav_mono(const char* filename) {
    unsigned int channels;
    unsigned int sampleRate;
    drwav_uint64 totalFrameCount;

    float* pcm = drwav_open_file_and_read_pcm_frames_f32(
        filename,
        &channels,
        &sampleRate,
        &totalFrameCount,
        nullptr
    );

    if (!pcm) {
        std::cerr << "Could not open WAV file\n";
        exit(1);
    }

    WavData out;
    out.sampleRate = sampleRate;
    out.channels = channels;
    out.frames = totalFrameCount;
    out.mono.resize((size_t)totalFrameCount);

    for (drwav_uint64 i = 0; i < totalFrameCount; i++) {
        float sum = 0.0f;

        for (unsigned int ch = 0; ch < channels; ch++) {
            sum += pcm[i * channels + ch];
        }

        out.mono[(size_t)i] = sum / channels;
    }

    drwav_free(pcm, nullptr);
    return out;
}

bool save_wav_mono(const char* filename, const std::vector<float>& samples, unsigned int sampleRate) {
    const std::filesystem::path path(filename);
    const std::filesystem::path parent = path.parent_path();

    if (!parent.empty()) {
        std::filesystem::create_directories(parent);
    }

    drwav_data_format format;
    format.container = drwav_container_riff;
    format.format = DR_WAVE_FORMAT_IEEE_FLOAT;
    format.channels = 1;
    format.sampleRate = sampleRate;
    format.bitsPerSample = 32;

    drwav wav;
    if (!drwav_init_file_write(&wav, filename, &format, nullptr)) {
        std::cerr << "Could not create WAV file " << filename << "\n";
        return false;
    }

    const drwav_uint64 framesWritten = drwav_write_pcm_frames(
        &wav,
        static_cast<drwav_uint64>(samples.size()),
        samples.data()
    );

    drwav_uninit(&wav);

    if (framesWritten != samples.size()) {
        std::cerr << "Could not write all WAV frames to " << filename << "\n";
        return false;
    }

    return true;
}
