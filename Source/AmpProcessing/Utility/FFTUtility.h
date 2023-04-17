#pragma once
#include <vector>

namespace AmpProcessing {
    namespace Convolution {
        class FFTUtility {
        public:
            inline static void FFT(std::vector<float>& real_part, std::vector<float>& imag_part) {
                const float PI = 3.14159f;

                int n = real_part.size();
                if (n <= 1) {
                    return;
                }

                // Divide input into even and odd halves
                std::vector<float> even_real(n / 2), even_imag(n / 2), odd_real(n / 2), odd_imag(n / 2);
                for (int i = 0; i < n / 2; i++) {
                    even_real[i] = real_part[2 * i];
                    even_imag[i] = imag_part[2 * i];
                    odd_real[i] = real_part[2 * i + 1];
                    odd_imag[i] = imag_part[2 * i + 1];
                }

                // Recursively compute FFT on even and odd halves
                FFT(even_real, even_imag);
                FFT(odd_real, odd_imag);

                // Combine FFTs of even and odd halves
                for (int i = 0; i < n / 2; i++) {
                    float twiddle_real = std::cos(-2 * PI * i / n);
                    float twiddle_imag = std::sin(-2 * PI * i / n);
                    float odd_real_tw = twiddle_real * odd_real[i] - twiddle_imag * odd_imag[i];
                    float odd_imag_tw = twiddle_real * odd_imag[i] + twiddle_imag * odd_real[i];
                    real_part[i] = even_real[i] + odd_real_tw;
                    imag_part[i] = even_imag[i] + odd_imag_tw;
                    real_part[i + n / 2] = even_real[i] - odd_real_tw;
                    imag_part[i + n / 2] = even_imag[i] - odd_imag_tw;
                }
            };

            inline static void IFFT(std::vector<float>& real_part, std::vector<float>& imag_part) {

            };
        };
    }
}