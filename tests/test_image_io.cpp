#include <catch.hpp>

#include "image.h"

#include <fstream>
#include <string>

bool CompareTwoFiles(std::string_view fileName1, std::string_view fileName2) {
    std::ifstream file1(fileName1.data(), std::ios_base::binary);
    std::ifstream file2(fileName2.data(), std::ios_base::binary);
    while (!file1.eof() && !file2.eof()) {
        if (file1.get() != file2.get()) {
            return false;
        }
    }
    return file1.eof() && file2.eof();
}

TEST_CASE("Test image IO") {
    std::string examplePath = EXAMPLE_PATH;
    Image image = Image::FromFile(examplePath);
    image.SaveToFile("/tmp/out.bmp");
    REQUIRE(CompareTwoFiles(EXAMPLE_PATH, "/tmp/out.bmp"));
}
