#include "filter.h"

#include <cmath>

namespace filters {

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
                    static_cast<size_t>(std::max(x + i, 0)),
                    image.Height() - 1);
            const size_t cur_y = std::min(
                    static_cast<size_t>(std::max(y + j, 0)), image.Width() - 1);
            result += image.GetPixel(cur_x, cur_y) *
                      GetMatrix()[i + radius][j + radius];
        }
    }
    result.Clamp();
    return result;
}

}  // namespace filters
