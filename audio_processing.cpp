#include "audio_processing.h"

// Function to process the audio signal using FXLMS algorithm
void audio_processing(hls::stream<audio_stream_t> &input,
                      hls::stream<audio_stream_t> &output,
                      hls::stream<audio_stream_t> &noise_input,
                      float gain,
                      const float secondary_path_coeffs[SEC_PATH_LEN]) {
    #pragma HLS INTERFACE axis port=input
    #pragma HLS INTERFACE axis port=output
    #pragma HLS INTERFACE axis port=noise_input
    #pragma HLS INTERFACE s_axilite port=gain
    #pragma HLS INTERFACE s_axilite port=return

    audio_stream_t input_sample, noise_sample, output_sample, noise_in;

    int16_t filter_coefficients[32] = {0};  // Initialize filter coefficients
    float step_size = 0.005;  // Step size for adaptation

    // Buffer to hold filtered reference signal
    float filtered_reference[SEC_PATH_LEN + 32] = {0.0f};

    while (!input.empty()) {
        input_sample = input.read();  // Read audio sample from input stream

        if (!noise_input.empty()) {
            noise_sample = noise_input.read();  // Read noise sample from noise input stream
        } else {
            noise_in.data = 0;  // If noise stream is empty, use zero
            noise_in.keep = 1;
            noise_in.strb = 1;
            noise_in.last = input_sample.last;  // Keep the last flag consistent with the input
            noise_sample = noise_in;  // Assign noise_in to noise_sample
        }

        // Compute the error signal
        int16_t error = noise_sample.data.to_int() - input_sample.data.to_int();  // Corrected error computation

        // Filter the reference signal by the secondary path
        for (int i = 0; i < SEC_PATH_LEN; i++) {
            filtered_reference[i] = 0;
            for (int j = 0; j <= i; j++) {
                if (j < 32) {
                    filtered_reference[i] += secondary_path_coeffs[i - j] * filter_coefficients[j];
                }
            }
        }

        // Update the filter coefficients using FXLMS algorithm
        for (int i = 0; i < 32; i++) {
            filter_coefficients[i] -= step_size * error * filtered_reference[i];
        }

        // Compute the output signal (anti-noise)
        int16_t processed_data = input_sample.data.to_int() - error;  // Corrected output computation

        // Ensure the result is within the 16-bit signed integer range
        if (processed_data > 32767) processed_data = 32767;
        else if (processed_data < -32768) processed_data = -32768;

        output_sample.data = processed_data;
        output_sample.keep = 1;
        output_sample.strb = 1;
        output_sample.last = input_sample.last;  // Keep the last flag consistent with the input

        output.write(output_sample);  // Send processed sample to output stream
    }
}

// Function to read a WAV file
std::vector<int16_t> read_wav(const std::string &filename, WAVHeader &header) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        exit(1);
    }

    // Read header
    file.read(reinterpret_cast<char *>(&header), sizeof(WAVHeader));

    // Check if format is valid (16-bit PCM)
    if (header.audio_format != 1 || header.bits_per_sample != 16) {
        std::cerr << "Error: Only 16-bit PCM WAV files are supported!" << std::endl;
        exit(1);
    }

    // Read audio samples
    std::vector<int16_t> samples(header.data_size / 2);
    file.read(reinterpret_cast<char *>(samples.data()), header.data_size);
    file.close();
    return samples;
}

// Function to write a WAV file
void write_wav(const std::string &filename, const WAVHeader &header, const std::vector<int16_t> &samples) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot write file " << filename << std::endl;
        exit(1);
    }

    // Write header
    file.write(reinterpret_cast<const char *>(&header), sizeof(WAVHeader));

    // Write audio samples
    file.write(reinterpret_cast<const char *>(samples.data()), samples.size() * sizeof(int16_t));
    file.close();
}
