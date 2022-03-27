#pragma once

#include "filter.h"

namespace filters {

class EdgeDetection : public MatrixFilter {
public:
    EdgeDetection(float threshold);

    Image operator()(const Image& image) const final;

private:
    static const inline std::vector<std::vector<float>> MATRIX{
            {0.,  -1., 0.},
            {-1., 4.,  -1.},
            {0.,  -1., 0.}};

    const std::vector<std::vector<float>>& GetMatrix() const final;

    float threshold_;
};

} // namespace filters
