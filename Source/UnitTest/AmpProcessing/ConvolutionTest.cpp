#include <gtest/gtest.h>
#include <Utility/ConvolutionUtility.h>

using namespace AmpProcessing::Convolution;

namespace UnitTest {
	TEST(ConvolutionShould, GiveBackExpectedVector_When_Invoked) {
        // Arrange
        const std::vector<float> buffer = { 1, 2, 3, 4 };
        const std::vector<float> ir = { 0.5, 1, 0.5 };
        const std::vector<float> expected_output = { 0.5, 2, 4, 6, 5.5, 2 };
        
        // Act
        auto output_buffer = ConvolutionUtility::Convolution(buffer, ir);

        // Assert
        // Check that the output matches the expected result
        ASSERT_EQ(output_buffer.size(), expected_output.size());
        for (int i = 0; i < output_buffer.size(); i++) {
            EXPECT_NEAR(output_buffer[i], expected_output[i], 1e-5);
        }
	}

    TEST(ConvolutionTest, OverlapAddConvolutionTest) {
        // Create input signal and impulse response
        std::vector<float> signal = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
        std::vector<float> ir = { 0.5, 0.25, -0.25, -0.5, 0.25, 0.75, -0.25 };

        // Calculate expected output using the naive Convolution function
        std::vector<float> expected_output = ConvolutionUtility::Convolution(signal, ir);

        // Perform overlap-add convolution
        int frame_size = 4;
        std::vector<float> output = ConvolutionUtility::OverlapAddConvolution(signal, ir, frame_size);

        // Check that output length is correct
        EXPECT_EQ(output.size(), expected_output.size());

        // Check that output values are close to expected values
        for (int i = 0; i < output.size(); i++) {
            EXPECT_NEAR(output[i], expected_output[i], 1e-6);
        }
    }
}