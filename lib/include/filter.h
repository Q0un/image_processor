#pragma once

#include "image.h"

#include <vector>

namespace filters {

class Filter {
public:
    virtual ~Filter() = default;
    virtual Image operator()(const Image& image) const = 0;
};

class MatrixFilter : public Filter {
public:
    Image operator()(const Image& image) const override;

protected:
    virtual const std::vector<std::vector<float>>& GetMatrix() const = 0;

private:
    Pixel ApplyMatrix(const Image& image, int32_t x, int32_t y) const;
};

} // namespace filters
