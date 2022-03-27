#pragma once

#include "filter.h"

#include <random>

namespace filters {

class Crystallize : public Filter {
public:
    Image operator()(const Image& image) const final;

private:
    static constexpr size_t CNT_POINTS = 1000;
    static inline auto rnd = std::mt19937(0);
};

} // namespace filters
