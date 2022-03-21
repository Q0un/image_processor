#include "image.h"

#include <stdexcept>

bool BMPFileHeader::Check() const {
    return fileType == 0x4D42 && reserved1 == 0 && reserved2 == 0;
}

bool BMPInfoHeader::Check() const {
    return planes == 1 && bitCount == 24 &&
           (compression == 0 || compression == 3);
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

    input.close();
    return image;
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
    if (image.infoHeader_.height < 0) {
        throw std::runtime_error("Height < 0");
    }
}

void Image::ReadData(Image& image, std::ifstream& input) {
    image.data_.resize(image.infoHeader_.width,
                       std::vector<Pixel>(image.infoHeader_.height));
    size_t rowPadding = (4 - image.infoHeader_.width * 3 % 4) % 4;
    for (size_t i = 0; i < image.infoHeader_.height; ++i) {
        for (size_t j = 0; j < image.infoHeader_.width; ++j) {
            uint8_t b = input.get();
            uint8_t g = input.get();
            uint8_t r = input.get();
            image.data_[i][j] = {b / 256.f, g / 256.f, r / 256.f};
        }
        uint8_t x;
        for (size_t j = 0; j < rowPadding; ++j) {
            input.read((char*) &x, sizeof(uint8_t));
        }
    }
    image.fileHeader_.fileSize += (image.infoHeader_.width * 3 + rowPadding) *
                                  image.infoHeader_.height;
}

void Image::SaveToFile(std::string_view fileName) const {
    std::ofstream output(fileName.data(), std::ios_base::binary);
    output.write((const char*) &fileHeader_, sizeof(BMPFileHeader));
    output.write((const char*) &infoHeader_, sizeof(BMPInfoHeader));
    size_t rowPadding = (4 - infoHeader_.width * 3 % 4) % 4;
    for (size_t i = 0; i < infoHeader_.height; ++i) {
        for (size_t j = 0; j < infoHeader_.width; ++j) {
            output.put(char(data_[i][j].b * 256));
            output.put(char(data_[i][j].g * 256));
            output.put(char(data_[i][j].r * 256));
        }
        for (size_t j = 0; j < rowPadding; ++j) {
            output.put(0);
        }
    }
    output.close();
}
