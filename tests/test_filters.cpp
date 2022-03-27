#include <catch.hpp>

#include "crop.h"
#include "grayscale.h"
#include "filter.h"
#include "image.h"
#include "negative.h"

#include <cmath>

class TestFilter : public filters::MatrixFilter {
private:
    static const inline std::vector<std::vector<float>> MATRIX{{0.5}};

    const std::vector<std::vector<float>>& GetMatrix() const final {
        return MATRIX;
    }
};

bool equals(float a, float b) {
    return std::abs(a - b) < 1e-6;
}

TEST_CASE("Test crop1") {
    const size_t width = 793;
    const size_t height = 636;
    Image image({}, {.width = width, .height = height}, {});
    filters::Crop filter(width, height);
    image = filter(image);
    REQUIRE(image.Width() == width);
    REQUIRE(image.Height() == height);
}

TEST_CASE("Test crop2") {
    const size_t width = 100;
    const size_t height = 100000;
    const size_t image_height = 20;
    Image image({}, {.width = width, .height = image_height}, {});
    filters::Crop filter(width, height);
    image = filter(image);
    REQUIRE(image.Width() == width);
    REQUIRE(image.Height() == image_height);
}

TEST_CASE("Test negative") {
    Image image({}, {.width = 1, .height = 1}, {{{0, 0.1, 0.35}}});
    filters::Negative filter;
    image = filter(image);
    Pixel res = image.GetPixel(0, 0);
    REQUIRE(equals(res.b, 1.f));
    REQUIRE(equals(res.g, 0.9f));
    REQUIRE(equals(res.r, 0.65f));
}

TEST_CASE("Test grayscale") {
    Image image({}, {.width = 1, .height = 1}, {{{0, 1, 1}}});
    filters::Grayscale filter;
    image = filter(image);
    Pixel res = image.GetPixel(0, 0);
    REQUIRE(equals(res.b, 0.886f));
    REQUIRE(equals(res.g, 0.886f));
    REQUIRE(equals(res.r, 0.886f));
}

TEST_CASE("Test matrix filter") {
    Image image({}, {.width = 1, .height = 1}, {{{0.5, 0.76, 0.12}}});
    TestFilter filter;
    image = filter(image);
    Pixel res = image.GetPixel(0, 0);
    REQUIRE(equals(res.b, 0.25f));
    REQUIRE(equals(res.g, 0.38f));
    REQUIRE(equals(res.r, 0.06f));
}
