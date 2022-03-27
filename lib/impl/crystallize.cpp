#include "crystallize.h"

namespace filters {

Image Crystallize::operator()(const Image& image) const {
    using Point = std::pair<size_t, size_t>;
    std::vector<Point> points;
    for (size_t i = 0; i < CNT_POINTS; ++i) {
        points.emplace_back(rnd() % image.Height(), rnd() % image.Width());
    }
    Image result = image;
    for (size_t x = 0; x < image.Height(); ++x) {
        for (size_t y = 0; y < image.Width(); ++y) {
            Point near = *std::min_element(points.begin(), points.end(),
                                           [x, y](const Point& l,
                                                  const Point& r) {
                                               return (l.first - x) *
                                                      (l.first - x) +
                                                      (l.second - y) *
                                                      (l.second - y) <
                                                      (r.first - x) *
                                                      (r.first - x) +
                                                      (r.second - y) *
                                                      (r.second - y);
                                           });
            result.SetPixel(x, y, image.GetPixel(near.first, near.second));
        }
    }
    return result;
}

} // namespace filters
