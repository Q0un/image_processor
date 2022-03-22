#include "filters.h"

namespace filters {

Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {}

Image Crop::operator()(const Image& image) const {
    Image result = image;
    result.Crop(width_, height_);
    return result;
}

Image Grayscale::operator()(const Image& image) const {
    Image result = image;
    for (size_t i = 0; i < image.Height(); ++i) {
        for (size_t j = 0; j < image.Width(); ++j) {
            result[i][j].r = result[i][j].g = result[i][j].b =
                    image[i][j].r * COEF_R + image[i][j].g * COEF_G +
                    image[i][j].b * COEF_B;
        }
    }
    return result;
}

Image Negative::operator()(const Image& image) const {
    Image result = image;
    for (size_t i = 0; i < image.Height(); ++i) {
        for (size_t j = 0; j < image.Width(); ++j) {
            result[i][j].r = 1 - result[i][j].r;
            result[i][j].g = 1 - result[i][j].g;
            result[i][j].b = 1 - result[i][j].b;
        }
    }
    return result;
}

Image MatrixFilter::operator()(const Image& image) const {
    Image result = image;
    for (size_t i = 0; i < image.Height(); ++i) {
        for (size_t j = 0; j < image.Width(); ++j) {
            result[i][j] = ApplyMatrix(image, i, j);
        }
    }
    return result;
}

Pixel MatrixFilter::ApplyMatrix(const Image& image, int32_t x,
                                int32_t y) const {
    Pixel result;
    for (int8_t i = -1; i <= 1; ++i) {
        for (int8_t j = -1; j <= 1; ++j) {
            int32_t curX = std::min(std::max(x + i, 0), image.Height() - 1);
            int32_t curY = std::min(std::max(y + j, 0), image.Width() - 1);
            result += image[curX][curY] * GetMatrix()[i + 1][j + 1];
        }
    }
    result.r = std::min(1.f, std::max(0.f, result.r));
    result.g = std::min(1.f, std::max(0.f, result.g));
    result.b = std::min(1.f, std::max(0.f, result.b));
    return result;
}

const std::vector<std::vector<float>>& Sharpening::GetMatrix() const {
    return MATRIX;
}

const std::vector<std::vector<float>>& EdgeDetection::GetMatrix() const {
    return MATRIX;
}

} // namespace filters
