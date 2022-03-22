#include "args_handler.h"

#include "filters.h"
#include "image.h"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

void ArgsHandler::Handle(size_t argc, char* argv[]) {
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
            const int32_t width = std::strtol(argv[++i], nullptr, 10);
            if (width <= 0) {
                throw std::runtime_error("Crop get wrong width");
            }
            const int32_t height = std::strtol(argv[++i], nullptr, 10);
            if (height <= 0) {
                throw std::runtime_error("Crop get wrong height");
            }
            const filters::Crop crop(width, height);
            image = crop(image);
        } else if (std::strcmp(argv[i], "-gs") == 0) {
            const filters::Grayscale grayscale;
            image = grayscale(image);
        } else if (std::strcmp(argv[i], "-neg") == 0) {
            const filters::Negative negative;
            image = negative(image);
        } else if (std::strcmp(argv[i], "-sharp") == 0) {
            const filters::Sharpening sharp;
            image = sharp(image);
        } else if (std::strcmp(argv[i], "-edge") == 0) {
            errno = 0;
            const float threshold = std::strtof(argv[++i], nullptr);
            if (errno) {
                throw std::runtime_error("Edge detection get wrong threshold");
            }
            const filters::EdgeDetection edge(threshold);
            image = edge(image);
        } else if (std::strcmp(argv[i], "-blur") == 0) {
            errno = 0;
            const float sigma = std::strtof(argv[++i], nullptr);
            if (errno) {
                throw std::runtime_error("Blur get wrong sigma");
            }
            const filters::GaussBlur blur(sigma);
            image = blur(image);
        } else if (std::strcmp(argv[i], "-rand") == 0) {
            const filters::RandomBlur blur;
            image = blur(image);
        } else {
            throw std::runtime_error("Wrong filter operand");
        }
    }
    image.SaveToFile(outputFile);
}

void ArgsHandler::WriteHelp() {
    std::cout
            << "Usage:\timage_processor INPUT_FILE OUTPUT_FILE [-FILTER1 PARAM1 ...] [-FILTER2 PARAM1 ...]"
            << std::endl;
    std::cout << std::endl;

    std::cout << "Apply several filters to bpm image" << std::endl;
    std::cout << std::endl;

    std::cout << "Filters:" << std::endl;
    std::cout << "  -crop WIDTH HEIGHT    Crop image" << std::endl;
    std::cout << "  -gs                   Apply grayscale" << std::endl;
    std::cout << "  -neg                  Apply negative" << std::endl;
    std::cout << "  -sharp                Apply sharpening" << std::endl;
    std::cout << "  -edge THRESHOLD       Apply edge detection" << std::endl;
    std::cout << "  -blur SIGMA           Apply gaussian blur" << std::endl;
}
