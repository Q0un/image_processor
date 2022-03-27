#pragma once

#include "filter.h"

namespace filters {

class GaussBlur : public Filter {
public:
    GaussBlur(float sigma);

    Image operator()(const Image& image) const final;

private:
    float sigma_;
    float denom_;
    float exp_;

    float ApplyFormula(size_t d) const;
};

} // namespace filters
