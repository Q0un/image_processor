#include "filters.h"

#include <cmath>

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
    result.Normalize();
    return result;
}

const std::vector<std::vector<float>>& Sharpening::GetMatrix() const {
    return MATRIX;
}

EdgeDetection::EdgeDetection(float threshold) : threshold_(threshold) {}

Image EdgeDetection::operator()(const Image& image) const {
    const Grayscale grayscale;
    Image result = grayscale(image);
    result = MatrixFilter::operator()(result);
    for (size_t i = 0; i < image.Height(); ++i) {
        for (size_t j = 0; j < image.Width(); ++j) {
            if (result[i][j].b > threshold_) {
                result[i][j] = {1, 1, 1};
            } else {
                result[i][j] = {0, 0, 0};
            }
        }
    }
    return result;
}

const std::vector<std::vector<float>>& EdgeDetection::GetMatrix() const {
    return MATRIX;
}

GaussBlur::GaussBlur(float sigma) : sigma_(sigma) {}

Image GaussBlur::operator()(const Image& image) const {
    Image result1 = image;
    for (size_t i = 0; i < image.Height(); ++i) {
        for (size_t j = 0; j < image.Width(); ++j) {
            size_t left = i >= RADIUS ? i - RADIUS : 0;
            size_t right =
                    i + RADIUS <= image.Height() ? i + RADIUS : image.Height();
            float sumR = 0;
            float sumG = 0;
            float sumB = 0;
            for (size_t k = left; k <= i; ++k) {
                sumR += ApplyFormula(image[k][j].r, i - k);
                sumG += ApplyFormula(image[k][j].g, i - k);
                sumB += ApplyFormula(image[k][j].b, i - k);
            }
            for (size_t k = i; k < right; ++k) {
                sumR += ApplyFormula(image[k][j].r, k - i);
                sumG += ApplyFormula(image[k][j].g, k - i);
                sumB += ApplyFormula(image[k][j].b, k - i);
            }
            result1[i][j] = {sumB, sumG, sumR};
        }
    }
    Image result2 = image;
    for (size_t i = 0; i < image.Height(); ++i) {
        for (size_t j = 0; j < image.Width(); ++j) {
            size_t left = j >= RADIUS ? j - RADIUS : 0;
            size_t right =
                    j + RADIUS <= image.Width() ? j + RADIUS : image.Width();
            float sumR = 0;
            float sumG = 0;
            float sumB = 0;
            for (size_t k = left; k <= j; ++k) {
                sumR += ApplyFormula(result1[i][k].r, j - k);
                sumG += ApplyFormula(result1[i][k].g, j - k);
                sumB += ApplyFormula(result1[i][k].b, j - k);
            }
            for (size_t k = j; k < right; ++k) {
                sumR += ApplyFormula(result1[i][k].r, k - j);
                sumG += ApplyFormula(result1[i][k].g, k - j);
                sumB += ApplyFormula(result1[i][k].b, k - j);
            }
            result2[i][j] = {sumB, sumG, sumR};
        }
    }
    for (size_t i = 0; i < image.Height(); ++i) {
        for (size_t j = 0; j < image.Width(); ++j) {
            result2[i][j].Normalize();
        }
    }
    return result2;
}

float GaussBlur::ApplyFormula(float x, size_t d) const {
    return x * std::exp(-(d * d / (2 * sigma_ * sigma_))) /
           std::sqrt(2 * std::numbers::pi * sigma_ * sigma_);
}

Image Glass::operator()(const Image& image) const {
    Image result = image;
    for (size_t i = 0; i < image.Height(); ++i) {
        for (size_t j = 0; j < image.Width(); ++j) {
            size_t leftX = i >= 10 ? i - 10 : 0;
            size_t rightX =
                    i + 10 < image.Height() ? i + 10 : image.Height() - 1;
            size_t leftY = j >= 10 ? j - 10 : 0;
            size_t rightY =
                    j + 10 < image.Width() ? j + 10 : image.Width() - 1;
            size_t cx = leftX + rnd() % (rightX - leftX);
            size_t cy = leftY + rnd() % (rightY - leftY);
            result[i][j] = image[cx][cy];
        }
    }
    return result;
}

} // namespace filters
