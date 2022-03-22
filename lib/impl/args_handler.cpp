#include "args_handler.h"

#include "filters.h"
#include "image.h"

#include <cstring>
#include <stdexcept>
#include <string>

void ArgsHandler::Handle(int argc, char* argv[]) {
    if (argc == 1) {
        WriteHelp();
        return;
    }
    if (argc < 3) {
        throw std::runtime_error("Missing operand");
    }
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    Image image = Image::FromFile(inputFile);
    for (size_t i = 3; i < argc; ++i) {
        if (std::strcmp(argv[i], "-crop") == 0) {
            const size_t width = std::strtoul(argv[++i], nullptr, 10);
            if (width == 0) {
                throw std::runtime_error("Crop get wrong width");
            }
            const size_t height = std::strtoul(argv[++i], nullptr, 10);
            if (height == 0) {
                throw std::runtime_error("Crop get wrong height");
            }
            const filters::Crop crop(width, height);
            image = crop(image);
        } else if (std::strcmp(argv[i], "-gs")) {
            const filters::Grayscale grayscale;
            image = grayscale(image);
        } else if (std::strcmp(argv[i], "-neg")) {
            const filters::Negative negative;
            image = negative(image);
        } else if (std::strcmp(argv[i], "-sharp")) {
            const filters::Sharpening sharp;
            image = sharp(image);
        } else if (std::strcmp(argv[i], "-edge")) {
            errno = 0;
            const float threshold = std::strtof(argv[++i], nullptr);
            if (errno) {
                throw std::runtime_error("Edge detection get wrong threshold");
            }
            const filters::EdgeDetection edge(threshold);
            image = edge(image);
        } else if (std::strcmp(argv[i], "-blur")) {
            errno = 0;
            const float sigma = std::strtof(argv[++i], nullptr);
            if (errno) {
                throw std::runtime_error("Blur get wrong sigma");
            }
            const filters::GaussBlur blur(sigma);
            image = blur(image);
        }
    }
}
