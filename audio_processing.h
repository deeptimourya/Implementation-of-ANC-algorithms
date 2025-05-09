#ifndef AUDIO_PROCESSING_H
#define AUDIO_PROCESSING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <ap_int.h>
#define SEC_PATH_LEN 32
using namespace std;

// Define 16-bit audio sample format
typedef ap_axis<16, 0, 0, 0> audio_stream_t;

// WAV file header structure (44-byte standard header)
struct WAVHeader {
    char riff[4];       // "RIFF"
    uint32_t size;      // File size
    char wave[4];       // "WAVE"
    char fmt[4];        // "fmt "
    uint32_t fmt_size;  // Format chunk size
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char data[4];       // "data"
    uint32_t data_size; // Size of audio data
};

// Function prototypes
std::vector<int16_t> read_wav(const std::string &filename, WAVHeader &header);
void write_wav(const std::string &filename, const WAVHeader &header, const std::vector<int16_t> &samples);
void audio_processing(hls::stream<audio_stream_t> &input,
                      hls::stream<audio_stream_t> &output,
                      hls::stream<audio_stream_t> &noise_input,
                      float gain,
                      const float secondary_path_coeffs[SEC_PATH_LEN]);

#endif  // AUDIO_PROCESSING_H
