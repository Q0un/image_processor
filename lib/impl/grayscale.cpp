#include "grayscale.h"

namespace filters {

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

} // namespace filters
