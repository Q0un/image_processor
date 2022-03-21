#pragma once

#include <fstream>
#include <string_view>
#include <vector>

#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offsetData;

    bool Check() const;
};

struct BMPInfoHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t sizeImage;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;

    bool Check() const;
};

#pragma pack(pop)

struct Pixel {
    float b;
    float g;
    float r;
};

class Image {
public:
    static Image FromFile(std::string_view fileName);
    static void ReadFileHeader(Image& image, std::ifstream& input);
    static void ReadInfoHeader(Image& image, std::ifstream& input);
    static void ReadData(Image& image, std::ifstream& input);

    void SaveToFile(std::string_view fileName) const;

private:
    BMPFileHeader fileHeader_;
    BMPInfoHeader infoHeader_;
    std::vector<std::vector<Pixel>> data_;
};
