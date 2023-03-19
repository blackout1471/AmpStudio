#include <vector>
#include <cmath>

namespace Filter {
    class FilterUtility {
    public:
        inline static std::vector<float> CreateDelay(int delay_samples, float feedback_gain) {
            // Create impulse response buffer
            std::vector<float> ir(delay_samples + 1, 0);

            // Set delay sample to 1
            ir[delay_samples] = 1.0f;

            // Set feedback sample to gain
            ir[0] = feedback_gain;

            return ir;
        }

        inline static std::vector<float> CreateReverb(float decay_time, int sample_rate, int length) {
            std::vector<float> ir(length);
            const float decay_factor = std::pow(10.0f, -3.0f * length / (decay_time * sample_rate));
            ir[0] = 1.0f;
            for (int i = 1; i < length; i++) {
                ir[i] = decay_factor * ir[i - 1];
            }
            return ir;
        }
    };
}