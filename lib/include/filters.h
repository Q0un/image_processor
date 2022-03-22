#pragma once

#include "image.h"

#include <vector>

namespace filters {

class Filter {
public:
    virtual Image operator()(const Image& image) const = 0;
};

class Crop : public Filter {
public:
    Crop(size_t width, size_t height);

    Image operator()(const Image& image) const final;

private:
    size_t width_;
    size_t height_;
};

class Grayscale : public Filter {
public:
    Image operator()(const Image& image) const final;

private:
    static constexpr float COEF_R = 0.299;
    static constexpr float COEF_G = 0.587;
    static constexpr float COEF_B = 0.114;
};

class Negative : public Filter {
public:
    Image operator()(const Image& image) const final;
};

class MatrixFilter : public Filter {
public:
    Image operator()(const Image& image) const final;

protected:
    virtual const std::vector<std::vector<float>>& GetMatrix() const = 0;

private:
    Pixel ApplyMatrix(const Image& image, int32_t x, int32_t y) const;
};

class Sharpening : public MatrixFilter {
private:
    static inline std::vector<std::vector<float>> MATRIX{{0.,  -1., 0.},
                                                          {-1., 5.,  -1.},
                                                          {0.,  -1., 0.}};

    const std::vector<std::vector<float>>& GetMatrix() const final;
};

class EdgeDetection : public MatrixFilter {
private:
    static inline std::vector<std::vector<float>> MATRIX{{0.,  -1., 0.},
                                                          {-1., 4.,  -1.},
                                                          {0.,  -1., 0.}};

    const std::vector<std::vector<float>>& GetMatrix() const final;
};

} // namespace filters
