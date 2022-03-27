#pragma once

#include <fstream>
#include <string_view>
#include <vector>

#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t file_type;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset_data;

    bool Check() const;
};

struct BMPInfoHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t size_image;
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t colors_important;

    bool Check() const;
};

#pragma pack(pop)

struct Pixel {
    float b{0};
    float g{0};
    float r{0};

    Pixel operator+(const Pixel& other) const;
    Pixel& operator+=(const Pixel& other);
    Pixel operator*(float x) const;
    Pixel& operator*=(float x);

    void Clamp();

    static const Pixel BLACK;
    static const Pixel WHITE;
};

class Image {
public:
    using DataType = std::vector<std::vector<Pixel>>;

    Image(const BMPFileHeader& file_header, const BMPInfoHeader& info_header,
          DataType data);

    size_t Width() const;
    size_t Height() const;
    size_t Padding() const;

    const Pixel& GetPixel(size_t x, size_t y) const;
    Pixel& SetPixel(size_t x, size_t y, const Pixel& pixel);

    void Crop(int32_t width, int32_t height);

    const BMPFileHeader& GetFileHeader() const;
    const BMPInfoHeader& GetInfoHeader() const;

private:
    BMPFileHeader file_header_;
    BMPInfoHeader info_header_;
    DataType data_;
};
