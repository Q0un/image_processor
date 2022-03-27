#include "bmp_reader.h"

namespace {

BMPFileHeader ReadFileHeader(std::ifstream& input) {
    BMPFileHeader result;
    input.read(reinterpret_cast<char*>(&result), sizeof(BMPFileHeader));
    if (!result.Check()) {
        throw std::runtime_error("Wrong file format");
    }
    return result;
}

BMPInfoHeader ReadInfoHeader(std::ifstream& input) {
    BMPInfoHeader result;
    input.read(reinterpret_cast<char*>(&result), sizeof(BMPInfoHeader));
    if (!result.Check()) {
        throw std::runtime_error("Wrong file format");
    }
    return result;
}

Image::DataType
ReadData(const BMPInfoHeader& info_header, std::ifstream& input) {
    const size_t height = info_header.height;
    const size_t width = info_header.width;
    Image::DataType result(height, std::vector<Pixel>(width));
    size_t rowPadding = (4 - width * 3 % 4) % 4;
    for (size_t x = 0; x < height; ++x) {
        for (size_t y = 0; y < width; ++y) {
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

}  // anonymous namespace

Image BMPReader::Read(std::string_view file_name) {
    std::ifstream input(file_name.data(), std::ios_base::binary);
    if (!input) {
        throw std::runtime_error("Unable to open input file");
    }
    auto file_header = ReadFileHeader(input);
    auto info_header = ReadInfoHeader(input);

    bool to_reverse = true;
    if (info_header.height < 0) {
        to_reverse = false;
        info_header.height *= -1;
    }

    input.seekg(file_header.offset_data, input.beg);
    auto data = ReadData(info_header, input);
    if (to_reverse) {
        std::reverse(data.begin(), data.end());
    }

    input.close();
    return {file_header, info_header, data};
}
