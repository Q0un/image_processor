#include <catch.hpp>

#include "image.h"

#include <fstream>
#include <string>

std::string EXAMPLES_PATH_STR(EXAMPLES_PATH);

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
    std::string examplePath = EXAMPLES_PATH_STR + "example.bmp";
    Image image = Image::FromFile(examplePath);
    image.SaveToFile("/tmp/out.bmp");
    REQUIRE(CompareTwoFiles(examplePath, "/tmp/out.bmp"));
}
