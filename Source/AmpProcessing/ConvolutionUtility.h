#include <vector>
class ConvolutionUtility {
public:
    inline static std::vector<float> Convolution(const std::vector<float>& buffer, const std::vector<float>& ir) {
        int buffer_size = buffer.size();
        int ir_size = ir.size();
        std::vector<float> output(buffer_size + ir_size - 1);
        int output_size = output.size();

        // Create temporary buffer for convolution
        std::vector<float> temp_buffer(output_size, 0);

        // Copy buffer to temp buffer
        std::copy(buffer.begin(), buffer.end(), temp_buffer.begin());

        // Convolve buffer with impulse response
        for (int i = 0; i < output_size; i++) {
            output[i] = 0;
            for (int j = 0; j < ir_size; j++) {
                if (i >= j && i - j < buffer_size) {
                    output[i] += temp_buffer[i - j] * ir[j];
                }
            }
        }

        return output;
    }
};