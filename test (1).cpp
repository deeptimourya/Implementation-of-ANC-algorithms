#include "audio_processing.h"
#include <string>
#include <vector>
#include <cstdint>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <ap_int.h>

int main() {
    std::string input_file = "C:\\Users\\hp\\Downloads\\file_example_WAV_2MG.wav"; // Provide actual input WAV file
    std::string noise_file = "C:\\Users\\hp\\Downloads\\audiocheck.net_whitenoisegaussian.wav"; // Provide actual noise WAV file
    std::string output_file = "C:\\Users\\hp\\Downloads\\output1.wav";

    WAVHeader input_header, noise_header;
    std::vector<int16_t> input_samples = read_wav(input_file, input_header);
    std::vector<int16_t> noise_samples = read_wav(noise_file, noise_header);
    std::vector<int16_t> output_samples;  // Ensure this is declared properly

    hls::stream<audio_stream_t> input_stream, output_stream, noise_stream;
    audio_stream_t sample_in, sample_out, noise_in;

    // Load input samples into AXI stream
    for (size_t i = 0; i < input_samples.size(); i++) {
        sample_in.data = input_samples[i];
        sample_in.keep = 1;
        sample_in.strb = 1;
        sample_in.last = (i == input_samples.size() - 1) ? 1 : 0;
        input_stream.write(sample_in);
    }

    // Load noise samples into AXI stream with scaling
    float noise_scale = 0.090;  // Scale down the noise by a factor
    for (size_t i = 0; i < input_samples.size(); i++) {
        if (i < noise_samples.size()) {
            int16_t scaled_noise = static_cast<int16_t>(noise_samples[i] * noise_scale);
            noise_in.data = scaled_noise;
        } else {
            noise_in.data = 0;  // Pad with zeros
        }
        noise_in.keep = 1;
        noise_in.strb = 1;
        noise_in.last = (i == input_samples.size() - 1) ? 1 : 0;
        noise_stream.write(noise_in);
    }

    // Example secondary path coefficients (replace with actual values)
   float secondary_path_coeffs[SEC_PATH_LEN] = {0.1, 0.2, 0.3, 0.4, 0.5};

    // Process samples one by one using FXLMS algorithm
    audio_processing(input_stream, output_stream, noise_stream, 15.0, secondary_path_coeffs);

    // Read output samples
    while (!output_stream.empty()) {
        sample_out = output_stream.read();
        output_samples.push_back(sample_out.data.to_int());
    }

    // Write processed samples to new WAV file
    write_wav(output_file, input_header, output_samples);

    std::cout << "Processing complete! Output saved to " << output_file << std::endl;
    return 0;
}
