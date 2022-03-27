#pragma once

#include "filter.h"

namespace filters {

class Crop : public Filter {
public:
    Crop(size_t width, size_t height);

    Image operator()(const Image& image) const final;

private:
    size_t width_;
    size_t height_;
};

} // namespace filters
