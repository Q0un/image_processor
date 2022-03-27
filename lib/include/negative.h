#pragma once

#include "filter.h"

namespace filters {

class Negative : public Filter {
public:
    Image operator()(const Image& image) const final;
};

} // namespace filters
