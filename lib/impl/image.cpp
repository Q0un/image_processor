#include "image.h"

#include <cmath>
#include <stdexcept>

bool BMPFileHeader::Check() const {
    return fileType == 0x4D42 && reserved1 == 0 && reserved2 == 0;
}

bool BMPInfoHeader::Check() const {
    return planes == 1 && bitCount == 24 &&
           (compression == 0 || compression == 3) && height > 0 && width > 0;
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

Image Image::FromFile(std::string_view fileName) {
    Image image;
    std::ifstream input(fileName.data(), std::ios_base::binary);
    if (!input) {
        throw std::runtime_error("Unable to open input file");
    }
    ReadFileHeader(image, input);
    ReadInfoHeader(image, input);

    input.seekg(image.fileHeader_.offsetData, input.beg);

    image.infoHeader_.size = sizeof(BMPInfoHeader);
    image.fileHeader_.fileSize = image.fileHeader_.offsetData =
            sizeof(BMPInfoHeader) + sizeof(BMPFileHeader);

    ReadData(image, input);

    if (image.infoHeader_.height > 0) {
        std::reverse(image.data_.begin(), image.data_.end());
    } else {
        image.infoHeader_.height *= -1;
    }

    input.close();
    return image;
}

void Image::SaveToFile(std::string_view fileName) const {
    std::ofstream output(fileName.data(), std::ios_base::binary);
    output.write((const char*) &fileHeader_, sizeof(BMPFileHeader));
    output.write((const char*) &infoHeader_, sizeof(BMPInfoHeader));
    size_t rowPadding = (4 - infoHeader_.width * 3 % 4) % 4;
    size_t i = infoHeader_.height - 1;
    do {
        for (size_t j = 0; j < infoHeader_.width; ++j) {
            output.put(static_cast<char>(data_[i][j].b * 255));
            output.put(static_cast<char>(data_[i][j].g * 255));
            output.put(static_cast<char>(data_[i][j].r * 255));
        }
        for (size_t j = 0; j < rowPadding; ++j) {
            output.put(0);
        }
    } while (i--);
    output.close();
}

std::vector<Pixel>& Image::operator[](size_t i) {
    return data_[i];
}

const std::vector<Pixel>& Image::operator[](size_t i) const {
    return data_[i];
}

int32_t Image::Width() const {
    return infoHeader_.width;
}

int32_t Image::Height() const {
    return infoHeader_.height;
}

void Image::Crop(int32_t width, int32_t height) {
    size_t rowPadding = (4 - infoHeader_.width * 3 % 4) % 4;
    fileHeader_.fileSize -=
            (infoHeader_.width * 3 + rowPadding) * infoHeader_.height;
    infoHeader_.width = std::min(infoHeader_.width, width);
    infoHeader_.height = std::min(infoHeader_.height, height);
    data_.erase(data_.begin() + infoHeader_.height, data_.end());
    for (size_t i = 0; i < infoHeader_.height; ++i) {
        data_[i].erase(data_[i].begin() + infoHeader_.width, data_[i].end());
    }
    rowPadding = (4 - infoHeader_.width * 3 % 4) % 4;
    fileHeader_.fileSize +=
            (infoHeader_.width * 3 + rowPadding) * infoHeader_.height;
}

void Image::ReadFileHeader(Image& image, std::ifstream& input) {
    input.read((char*) &image.fileHeader_, sizeof(BMPFileHeader));
    if (!image.fileHeader_.Check()) {
        throw std::runtime_error("Wrong file format");
    }
}

void Image::ReadInfoHeader(Image& image, std::ifstream& input) {
    input.read((char*) &image.infoHeader_, sizeof(BMPInfoHeader));
    if (!image.infoHeader_.Check()) {
        throw std::runtime_error("Wrong file format");
    }
}

void Image::ReadData(Image& image, std::ifstream& input) {
    image.data_.resize(image.infoHeader_.width,
                       std::vector<Pixel>(image.infoHeader_.height));
    size_t rowPadding = (4 - image.infoHeader_.width * 3 % 4) % 4;
    for (size_t i = 0; i < std::abs(image.infoHeader_.height); ++i) {
        for (size_t j = 0; j < image.infoHeader_.width; ++j) {
            uint8_t b = input.get();
            uint8_t g = input.get();
            uint8_t r = input.get();
            image.data_[i][j] = {b / 255.f, g / 255.f, r / 255.f};
        }
        uint8_t x;
        for (size_t j = 0; j < rowPadding; ++j) {
            input.read((char*) &x, sizeof(uint8_t));
        }
    }
    image.fileHeader_.fileSize += (image.infoHeader_.width * 3 + rowPadding) *
                                  image.infoHeader_.height;
}
