#include <gtest/gtest.h>
#include <Utility/FFTUtility.h>

using namespace AmpProcessing::Convolution;

namespace UnitTest {
    TEST(FFTShould, ExpectRealNumbers_When_InvokingFFT) {
        // Arrange
        std::vector<float> real_buffer = { 1, 2, 3, 4, 5 };
        std::vector<float> imagine_buffer = { 0, 0, 0, 0, 0 };

        std::vector<float> expected_real = { 15.000000, -5.414214, 3.000000, -2.585786, 3.000000, -2.585786, 3.000000, -5.414214 };
        int expected_size = 8;

        // Act
        FFTUtility::FFT(real_buffer, imagine_buffer);

        // Assert
        // Check that the output matches the expected result
        ASSERT_EQ(real_buffer.size(), expected_size);
        for (int i = 0; i < expected_size; i++) {
            EXPECT_NEAR(real_buffer[i], expected_real[i], 1e-5);
        }
    }

    TEST(FFTShould, ExpectImagineNumbers_When_InvokingFFT) {
        // Arrange
        std::vector<float> real_buffer = { 1, 2, 3, 4, 5 };
        std::vector<float> imagine_buffer = { 0, 0, 0, 0, 0 };

        std::vector<float> expected_imagine = { 0.000000, -7.242641, 2.000000, -1.242641, 0.000000, 1.242641, -2.000000, 7.242641 };
        int expected_size = 8;

        // Act
        FFTUtility::FFT(real_buffer, imagine_buffer);

        // Assert
        // Check that the output matches the expected result
        ASSERT_EQ(imagine_buffer.size(), expected_size);
        for (int i = 0; i < expected_size; i++) {
            EXPECT_NEAR(imagine_buffer[i], expected_imagine[i], 1e-5);
        }
    }
}