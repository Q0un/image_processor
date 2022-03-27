#pragma once

#include "filter.h"

namespace filters {

class Grayscale : public Filter {
public:
    Image operator()(const Image& image) const final;

private:
    static constexpr float COEF_R = 0.299;
    static constexpr float COEF_G = 0.587;
    static constexpr float COEF_B = 0.114;
};

} // namespace filters
