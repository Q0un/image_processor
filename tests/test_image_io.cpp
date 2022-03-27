#include <catch.hpp>

#include "bmp_reader.h"
#include "bmp_writer.h"
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
    const std::string examplePath = EXAMPLE_PATH;
    const Image image = BMPReader::Read(examplePath);
    BMPWriter::Write("/tmp/out.bmp", image);
    REQUIRE(CompareTwoFiles(EXAMPLE_PATH, "/tmp/out.bmp"));
}
