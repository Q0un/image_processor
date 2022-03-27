#pragma once

#include "image.h"

#include <string_view>

class BMPReader {
public:
    static Image Read(std::string_view file_name);
};