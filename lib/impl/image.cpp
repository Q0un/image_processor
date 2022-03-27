#include "image.h"

#include <cmath>
#include <stdexcept>

bool BMPFileHeader::Check() const {
    return file_type == 0x4D42;
}

bool BMPInfoHeader::Check() const {
    return planes == 1 && bit_count == 24 && compression == 0;
}

Pixel Pixel::operator+(const Pixel& other) const {
    Pixel result = *this;
    result += other;
    return result;
}

Pixel& Pixel::operator+=(const Pixel& other) {
    b += other.b;
    g += other.g;
    r += other.r;
    return *this;
}

Pixel Pixel::operator*(float x) const {
    Pixel result = *this;
    result *= x;
    return result;
}

Pixel& Pixel::operator*=(float x) {
    b *= x;
    g *= x;
    r *= x;
    return *this;
}

void Pixel::Clamp() {
    r = std::min(1.f, std::max(0.f, r));
    g = std::min(1.f, std::max(0.f, g));
    b = std::min(1.f, std::max(0.f, b));
}

const Pixel Pixel::BLACK = {0, 0, 0};
const Pixel Pixel::WHITE = {1, 1, 1};

Image::Image(const BMPFileHeader& file_header, const BMPInfoHeader& info_header,
             DataType data) : file_header_(file_header),
                              info_header_(info_header),
                              data_(std::move(data)) {}

size_t Image::Width() const {
    return info_header_.width;
}

size_t Image::Height() const {
    return info_header_.height;
}

size_t Image::Padding() const {
    return (4 - Width() * 3 % 4) % 4;
}

const Pixel& Image::GetPixel(size_t x, size_t y) const {
    return data_[x][y];
}

Pixel& Image::SetPixel(size_t x, size_t y, const Pixel& pixel) {
    return data_[x][y] = pixel;
}

void Image::Crop(int32_t width, int32_t height) {
    size_t rowPadding = Padding();
    file_header_.file_size -= (Width() * 3 + rowPadding) * Height();
    info_header_.width = std::min(info_header_.width, width);
    info_header_.height = std::min(info_header_.height, height);
    data_.resize(Height());
    for (size_t x = 0; x < Height(); ++x) {
        data_[x].resize(Width());
    }
    rowPadding = Padding();
    file_header_.file_size += (Width() * 3 + rowPadding) * Height();
}

const BMPFileHeader& Image::GetFileHeader() const {
    return file_header_;
}

const BMPInfoHeader& Image::GetInfoHeader() const {
    return info_header_;
}
