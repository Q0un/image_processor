#include "gauss_blur.h"

#include <cmath>

namespace filters {

GaussBlur::GaussBlur(float sigma) {
    sigma_ = sigma;
    denom_ = std::sqrt(2 * std::numbers::pi * sigma_ * sigma_);
    exp_ = std::exp(-(1. / (2 * sigma_ * sigma_)));
}

Image GaussBlur::operator()(const Image& image) const {
    Image result1 = image;
    const size_t radius = std::ceil(6 * sigma_);
    for (size_t x = 0; x < image.Height(); ++x) {
        for (size_t y = 0; y < image.Width(); ++y) {
            const size_t down = x >= radius ? x - radius : 0;
            const size_t up = x + radius + 1 <= image.Height() ? x + radius + 1
                                                               : image.Height();
            float sum_r = 0;
            float sum_g = 0;
            float sum_b = 0;
            for (size_t k = down; k < up; ++k) {
                const auto image_pixel = image.GetPixel(k, y);
                const size_t d = x >= k ? x - k : k - x;
                const float coef = ApplyFormula(d);
                sum_r += image_pixel.r * coef;
                sum_g += image_pixel.g * coef;
                sum_b += image_pixel.b * coef;
            }
            result1.SetPixel(x, y, {sum_b, sum_g, sum_r});
        }
    }
    Image result2 = image;
    for (size_t x = 0; x < image.Height(); ++x) {
        for (size_t y = 0; y < image.Width(); ++y) {
            const size_t left = y >= radius ? y - radius : 0;
            const size_t right = y + radius + 1 <= image.Width()
                                 ? y + radius + 1
                                 : image.Width();
            float sum_r = 0;
            float sum_g = 0;
            float sum_b = 0;
            for (size_t k = left; k < right; ++k) {
                const auto image_pixel = result1.GetPixel(x, k);
                const size_t d = y >= k ? y - k : k - y;
                const float coef = ApplyFormula(d);
                sum_r += image_pixel.r * coef;
                sum_g += image_pixel.g * coef;
                sum_b += image_pixel.b * coef;
            }
            Pixel pixel = {sum_b, sum_g, sum_r};
            pixel.Clamp();
            result2.SetPixel(x, y, pixel);
        }
    }
    return result2;
}

float GaussBlur::ApplyFormula(size_t d) const {
    return std::pow(exp_, d * d) / denom_;
}

} // namespace filters
