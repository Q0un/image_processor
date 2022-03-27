#include "crop.h"

namespace filters {

Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {
}

Image Crop::operator()(const Image& image) const {
    Image result = image;
    result.Crop(width_, height_);
    return result;
}

} // namespace filters
