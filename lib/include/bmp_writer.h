#pragma once

#include "image.h"

#include <string_view>

class BMPWriter {
public:
    static void Write(std::string_view file_name, const Image& image);
};