#include "negative.h"

namespace filters {

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

} // namespace filters
