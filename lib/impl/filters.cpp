#include "filters.h"

#include <cmath>

namespace filters {

Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {
}

Image Crop::operator()(const Image& image) const {
    Image result = image;
    result.Crop(width_, height_);
    return result;
}

Image Grayscale::operator()(const Image& image) const {
    Image result = image;
    for (size_t x = 0; x < image.Height(); ++x) {
        for (size_t y = 0; y < image.Width(); ++y) {
            Pixel image_pixel = image.GetPixel(x, y);
            auto value = image_pixel.r * COEF_R + image_pixel.g * COEF_G +
                         image_pixel.b * COEF_B;
            result.SetPixel(x, y, {value, value, value});
        }
    }
    return result;
}

Image Negative::operator()(const Image& image) const {
    Image result = image;
    for (size_t x = 0; x < image.Height(); ++x) {
        for (size_t y = 0; y < image.Width(); ++y) {
            Pixel image_pixel = image.GetPixel(x, y);
            result.SetPixel(
                x, y,
                {1 - image_pixel.b, 1 - image_pixel.g, 1 - image_pixel.r});
        }
    }
    return result;
}

Image MatrixFilter::operator()(const Image& image) const {
    Image result = image;
    for (size_t x = 0; x < image.Height(); ++x) {
        for (size_t y = 0; y < image.Width(); ++y) {
            result.SetPixel(x, y, ApplyMatrix(image, x, y));
        }
    }
    return result;
}

Pixel MatrixFilter::ApplyMatrix(const Image& image, int32_t x,
                                int32_t y) const {
    Pixel result;
    const int8_t radius = GetMatrix().size() / 2;
    for (int8_t i = -radius; i <= radius; ++i) {
        for (int8_t j = -radius; j <= radius; ++j) {
            const size_t cur_x = std::min(
                static_cast<size_t>(std::max(x + i, 0)), image.Height() - 1);
            const size_t cur_y = std::min(
                static_cast<size_t>(std::max(y + j, 0)), image.Width() - 1);
            result += image.GetPixel(cur_x, cur_y) * GetMatrix()[i + 1][j + 1];
        }
    }
    result.Clamp();
    return result;
}

const std::vector<std::vector<float>>& Sharpening::GetMatrix() const {
    return MATRIX;
}

EdgeDetection::EdgeDetection(float threshold) : threshold_(threshold) {
}

Image EdgeDetection::operator()(const Image& image) const {
    const Grayscale grayscale;
    Image result = grayscale(image);
    result = MatrixFilter::operator()(result);
    for (size_t x = 0; x < image.Height(); ++x) {
        for (size_t y = 0; y < image.Width(); ++y) {
            if (result.GetPixel(x, y).b > threshold_) {
                result.SetPixel(x, y, Pixel::WHITE);
            } else {
                result.SetPixel(x, y, Pixel::BLACK);
            }
        }
    }
    return result;
}

const std::vector<std::vector<float>>& EdgeDetection::GetMatrix() const {
    return MATRIX;
}

GaussBlur::GaussBlur(float sigma) : sigma_(sigma) {
}

Image GaussBlur::operator()(const Image& image) const {
    Image result1 = image;
    for (size_t x = 0; x < image.Height(); ++x) {
        for (size_t y = 0; y < image.Width(); ++y) {
            const size_t down = x >= RADIUS ? x - RADIUS : 0;
            const size_t up = x + RADIUS + 1 <= image.Height() ? x + RADIUS + 1
                                                               : image.Height();
            float sum_r = 0;
            float sum_g = 0;
            float sum_b = 0;
            for (size_t k = down; k < up; ++k) {
                const auto image_pixel = image.GetPixel(k, y);
                const size_t d = x >= k ? x - k : k - x;
                sum_r += ApplyFormula(image_pixel.r, d);
                sum_g += ApplyFormula(image_pixel.g, d);
                sum_b += ApplyFormula(image_pixel.b, d);
            }
            result1.SetPixel(x, y, {sum_b, sum_g, sum_r});
        }
    }
    Image result2 = image;
    for (size_t x = 0; x < image.Height(); ++x) {
        for (size_t y = 0; y < image.Width(); ++y) {
            const size_t left = y >= RADIUS ? y - RADIUS : 0;
            const size_t right = y + RADIUS + 1 <= image.Width()
                                     ? y + RADIUS + 1
                                     : image.Width();
            float sum_r = 0;
            float sum_g = 0;
            float sum_b = 0;
            for (size_t k = left; k < right; ++k) {
                const auto image_pixel = result1.GetPixel(x, k);
                const size_t d = y >= k ? y - k : k - y;
                sum_r += ApplyFormula(image_pixel.r, d);
                sum_g += ApplyFormula(image_pixel.g, d);
                sum_b += ApplyFormula(image_pixel.b, d);
            }
            Pixel pixel = {sum_b, sum_g, sum_r};
            pixel.Clamp();
            result2.SetPixel(x, y, pixel);
        }
    }
    return result2;
}

float GaussBlur::ApplyFormula(float x, size_t d) const {
    return x * std::exp(-(d * d / (2 * sigma_ * sigma_))) /
           std::sqrt(2 * std::numbers::pi * sigma_ * sigma_);
}

Image RandomBlur::operator()(const Image& image) const {
    Image result = image;
    for (size_t x = 0; x < image.Height(); ++x) {
        for (size_t y = 0; y < image.Width(); ++y) {
            size_t left_x = x >= RADIUS ? x - RADIUS : 0;
            size_t right_x =
                x + RADIUS < image.Height() ? x + RADIUS : image.Height() - 1;
            size_t left_y = y >= RADIUS ? y - RADIUS : 0;
            size_t right_y =
                y + RADIUS < image.Width() ? y + RADIUS : image.Width() - 1;
            size_t cx = left_x + rnd() % (right_x - left_x);
            size_t cy = left_y + rnd() % (right_y - left_y);
            result.SetPixel(x, y, image.GetPixel(cx, cy));
        }
    }
    return result;
}

}  // namespace filters
