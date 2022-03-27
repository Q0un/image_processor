#pragma once

#include "filter.h"

namespace filters {

class Sharpening : public MatrixFilter {
private:
    static const inline std::vector<std::vector<float>> MATRIX{
            {0.,  -1., 0.},
            {-1., 5.,  -1.},
            {0.,  -1., 0.}};

    const std::vector<std::vector<float>>& GetMatrix() const final;
};

} // namespace filters
