#include "edge_detection.h"
#include "grayscale.h"

namespace filters {

EdgeDetection::EdgeDetection(float threshold) : threshold_(threshold) {
}

Image EdgeDetection::operator()(const Image& image) const {
    static const Grayscale grayscale;
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

} // namespace filters
