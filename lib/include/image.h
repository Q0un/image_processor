#pragma once

#include <string_view>
#include <vector>

struct Pixel {
    float r;
    float g;
    float b;
};

class Image {
public:
    static Image FromFile(std::string_view fileName);

private:
    std::vector<std::vector<Pixel>> data_;
};