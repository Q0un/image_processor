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
    return {b + other.b, g + other.g, r + other.r};
}

Pixel& Pixel::operator+=(const Pixel& other) {
    return (*this) = operator+(other);
}

Pixel Pixel::operator*(float x) const {
    return {b * x, g * x, r * x};
}

Pixel& Pixel::operator*=(float x) {
    return (*this) = operator*(x);
}

void Pixel::Clamp() {
    r = std::min(1.f, std::max(0.f, r));
    g = std::min(1.f, std::max(0.f, g));
    b = std::min(1.f, std::max(0.f, b));
}

const Pixel Pixel::BLACK = {0, 0, 0};
const Pixel Pixel::WHITE = {1, 1, 1};

namespace {

BMPFileHeader ReadFileHeader(std::ifstream& input) {
    BMPFileHeader result;
    input.read(reinterpret_cast<char*>(&result), sizeof(BMPFileHeader));
    if (!result.Check()) {
        throw std::runtime_error("Wrong file format");
    }
    return result;
}

BMPInfoHeader ReadInfoHeader(Image& image, std::ifstream& input) {
    BMPInfoHeader result;
    input.read(reinterpret_cast<char*>(&result), sizeof(BMPInfoHeader));
    if (!result.Check()) {
        throw std::runtime_error("Wrong file format");
    }
    return result;
}

Image::DataType ReadData(const Image& image, std::ifstream& input) {
    Image::DataType result(image.Width(), std::vector<Pixel>(image.Height()));
    size_t rowPadding = (4 - image.Width() * 3 % 4) % 4;
    for (size_t x = 0; x < image.Height(); ++x) {
        for (size_t y = 0; y < image.Width(); ++y) {
            uint8_t b = input.get();
            uint8_t g = input.get();
            uint8_t r = input.get();
            result[x][y] = {b / 255.f, g / 255.f, r / 255.f};
        }
        for (size_t i = 0; i < rowPadding; ++i) {
            input.get();
        }
    }
    return result;
}

} // anonymous namespace

Image Image::FromFile(std::string_view fileName) {
    Image image;
    std::ifstream input(fileName.data(), std::ios_base::binary);
    if (!input) {
        throw std::runtime_error("Unable to open input file");
    }
    image.file_header_ = ReadFileHeader(input);
    image.info_header_ = ReadInfoHeader(image, input);

    bool to_reverse = true;
    if (image.info_header_.height < 0) {
        to_reverse = false;
        image.info_header_.height *= -1;
    }

    input.seekg(image.file_header_.offset_data, input.beg);
    image.data_ = ReadData(image, input);
    if (to_reverse) {
        std::reverse(image.data_.begin(), image.data_.end());
    }

    input.close();
    return image;
}

void Image::SaveToFile(std::string_view fileName) const {
    std::ofstream output(fileName.data(), std::ios_base::binary);
    output.write(reinterpret_cast<const char*>(&file_header_),
                 sizeof(BMPFileHeader));
    output.write(reinterpret_cast<const char*>(&info_header_),
                 sizeof(BMPInfoHeader));
    size_t rowPadding = (4 - info_header_.width * 3 % 4) % 4;
    size_t x = Height() - 1;
    do {
        for (size_t y = 0; y < Width(); ++y) {
            output.put(static_cast<char>(data_[x][y].b * 255));
            output.put(static_cast<char>(data_[x][y].g * 255));
            output.put(static_cast<char>(data_[x][y].r * 255));
        }
        for (size_t j = 0; j < rowPadding; ++j) {
            output.put(0);
        }
    } while (x--);
    output.close();
}

size_t Image::Width() const {
    return info_header_.width;
}

size_t Image::Height() const {
    return info_header_.height;
}

const Pixel& Image::GetPixel(size_t x, size_t y) const {
    return data_[x][y];
}

Pixel& Image::SetPixel(size_t x, size_t y, const Pixel& pixel) {
    return data_[x][y] = pixel;
}

void Image::Crop(int32_t width, int32_t height) {
    size_t rowPadding = (4 - info_header_.width * 3 % 4) % 4;
    file_header_.file_size -=
        (info_header_.width * 3 + rowPadding) * info_header_.height;
    info_header_.width = std::min(info_header_.width, width);
    info_header_.height = std::min(info_header_.height, height);
    data_.erase(data_.begin() + info_header_.height, data_.end());
    for (size_t x = 0; x < Height(); ++x) {
        data_[x].resize(info_header_.width);
    }
    rowPadding = (4 - info_header_.width * 3 % 4) % 4;
    file_header_.file_size +=
        (info_header_.width * 3 + rowPadding) * info_header_.height;
}
