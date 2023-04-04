#include <vector>
#include <xmmintrin.h>

namespace AmpProcessing {
    namespace Convolution {

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

            inline static std::vector<float> ConvolutionSIMD(const std::vector<float>& buffer, std::vector<float>& ir) {
                int buffer_size = buffer.size();
                int ir_size = ir.size();
                std::vector<float> output(buffer_size + ir_size - 1);
                int output_size = output.size();

                // Create temporary buffer for convolution
                std::vector<float> temp_buffer(output_size, 0);

                // Copy buffer to temp buffer
                std::copy(buffer.begin(), buffer.end(), temp_buffer.begin());

                // Convolve buffer with impulse response using SSE instructions
                __m128 buffer_data, ir_data, output_data, temp_data;
                float* buffer_ptr = &temp_buffer[0];
                float* ir_ptr = &ir[0];
                float* output_ptr = &output[0];
                int j;

                for (int i = 0; i < output_size; i++) {
                    output_data = _mm_setzero_ps(); // set output_data to zero
                    for (j = 0; j < ir_size - 3; j += 4) {
                        buffer_data = _mm_loadu_ps(buffer_ptr + i - j); // load 4 values from buffer
                        ir_data = _mm_loadu_ps(ir_ptr + j); // load 4 values from impulse response
                        output_data = _mm_add_ps(output_data, _mm_mul_ps(buffer_data, ir_data)); // multiply and add using SSE instructions
                    }

                    // Handle the remaining elements using scalar code
                    for (; j < ir_size; j++) {
                        if (i >= j && i - j < buffer_size) {
                            output_data.m128_f32[0] += temp_buffer[i - j] * ir[j];
                        }
                    }

                    // Store the result to output buffer
                    _mm_store_ss(output_ptr + i, output_data);
                }

                return output;
            }

            inline static void ProcessBuffer(const std::vector<float>& input_block, const std::vector<float>& filter_kernel,
                std::vector<float>& output_block, std::vector<float>& overlap) {

                int block_size = input_block.size();
                int ir_size = filter_kernel.size();

                // Pad the input block and overlap with the previous block
                std::vector<float> padded_block(block_size + ir_size - 1);
                for (int i = 0; i < block_size; i++) {
                    padded_block[i] = input_block[i];
                }
                for (int i = 0; i < ir_size - 1; i++) {
                    padded_block[block_size + i] = overlap[i];
                }

                // Apply the convolution operation to the padded block
                std::vector<float> padded_output_block = ConvolutionUtility::Convolution(padded_block, filter_kernel);

                // Overlap and add with the previous block's output
                for (int i = 0; i < block_size; i++) {
                    output_block[i] = padded_output_block[i] + overlap[i];
                }

                // Update the overlap buffer
                for (int i = 0; i < ir_size - 1; i++) {
                    overlap[i] = padded_output_block[block_size + i];
                }
            }
        };
    }
}
