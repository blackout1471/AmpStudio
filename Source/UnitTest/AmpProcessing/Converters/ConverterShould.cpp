#include <gtest/gtest.h>
#include <Converters/Int32LsbAudioConverter.h>

using namespace AmpProcessing::Converters;

namespace UnitTest {
    TEST(Int32LsbConverterShould, ReturnExpectedFloat_When_ConvertingToFloat) {
        // Arrange
        int32_t from[] = { 8760, 20000 };
        std::vector<float> expected{ 4.0791928768157959e-06f, 9.31322575e-06f};
        std::vector<float> actual = { 0.f, 0.f };
        auto converter = Int32LsbAudioConverter();

        // Act
        converter.ConvertToFloat(from, actual, expected.size());

        // Assert
        for (size_t i = 0; i < actual.size(); i++)
        {
            EXPECT_NEAR(actual[i], expected[i], 1e-5);
        }
    }

    TEST(Int32LsbConverterShould, ReturnExpectedFloat_When_ConvertingToInt) {
        // Arrange
        int32_t expected[] = { 8760, 20000 };
        std::vector<float> from { 4.079192878715e-06f, 9.313225750491e-06f };
        int32_t actual[] = {0.f, 0.f};
        auto converter = Int32LsbAudioConverter();

        // Act
        converter.ConvertFromFloat(from, actual);

        // Assert
        for (size_t i = 0; i < from.size(); i++)
        {
            EXPECT_NEAR(actual[i], expected[i], 1e-5);
        }
    }
}