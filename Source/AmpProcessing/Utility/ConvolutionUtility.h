#include <vector>
#include <algorithm>
#include <cmath>

namespace AmpProcessing {
    namespace Convolution {

        class ConvolutionUtility {
        public:
            inline static std::vector<float> Convolution(const std::vector<float>& buffer, const std::vector<float>& ir) {
                auto buffer_size = buffer.size();
                auto ir_size = ir.size();
                std::vector<float> output(buffer_size + ir_size - 1);
                auto output_size = output.size();

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

            inline static std::vector<float> OverlapAddConvolution(const std::vector<float>& buffer, const std::vector<float>& ir, size_t segment_size) {
                auto buffer_size = buffer.size();
                auto ir_size = ir.size();
                std::vector<float> output(buffer_size + ir_size - 1);
                size_t num_segments = std::ceil(static_cast<float>(buffer_size) / segment_size);

                for (size_t n = 0; n < num_segments; ++n) {
                    size_t segment_start = n * segment_size;
                    size_t segment_end = std::min<size_t>(segment_start + segment_size, buffer_size);

                    std::vector<float> segment_buffer(buffer.begin() + segment_start, buffer.begin() + segment_end);
                    std::vector<float> convolved_segment = Convolution(segment_buffer, ir);

                    for (size_t i = 0; i < convolved_segment.size(); ++i) {
                        output[segment_start + i] += convolved_segment[i];
                    }
                }

                return output;
            }
        };
    }
}
