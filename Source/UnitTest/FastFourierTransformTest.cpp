#include <gtest/gtest.h>
#include <Utility/FFTUtility.h>

using namespace AmpProcessing::Convolution;

namespace UnitTest {
    TEST(FFTShould, ExpectRealNumbers_When_InvokingFFT) {
        // Arrange
        std::vector<float> real_buffer      = { 1, 2, 3, 4 };
        std::vector<float> imagine_buffer   = { 0, 0, 0, 0 };

        std::vector<float> expected_real = { 10.000000, -2.000000, -2.000000, -2.000000 };
        int expected_size = 4;

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
        std::vector<float> real_buffer = { 1, 2, 3, 4 };
        std::vector<float> imagine_buffer = { 0, 0, 0, 0 };

        std::vector<float> expected_imagine = { 0.000000, 2.000000, 0.000000, - 2.000000 };
        int expected_size = 4;

        // Act
        FFTUtility::FFT(real_buffer, imagine_buffer);

        // Assert
        // Check that the output matches the expected result
        ASSERT_EQ(imagine_buffer.size(), expected_size);
        for (int i = 0; i < expected_size; i++) {
            EXPECT_NEAR(imagine_buffer[i], expected_imagine[i], 1e-5);
        }
    }

    TEST(FFTShould, ExpectRealNumbers_When_InvokingIFFT) {
        // Arrange
        std::vector<float> real_buffer      = { 10.00000, -2.000000, -2.000000, -2.000000 };
        std::vector<float> imagine_buffer   = { 0.000000, 2.000000, 0.000000, -2.000000 };

        std::vector<float> expected_real = { 1, 2, 3, 4 };
        int expected_size = 4;

        // Act
        FFTUtility::IFFT(real_buffer, imagine_buffer);

        // Assert
        // Check that the output matches the expected result
        ASSERT_EQ(real_buffer.size(), expected_size);
        for (int i = 0; i < expected_size; i++) {
            EXPECT_NEAR(real_buffer[i], expected_real[i], 1e-5);
        }
    }
}