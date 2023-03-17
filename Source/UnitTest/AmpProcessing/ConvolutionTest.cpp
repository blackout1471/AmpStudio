#include <gtest/gtest.h>
#include <ConvolutionUtility.h>

using namespace Convolution;

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
}