#include "args_handler.h"

#include "bmp_reader.h"
#include "bmp_writer.h"
#include "crop.h"
#include "crystallize.h"
#include "edge_detection.h"
#include "gauss_blur.h"
#include "grayscale.h"
#include "image.h"
#include "negative.h"
#include "sharpening.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

void ArgsHandler::Handle(size_t argc, char* argv[]) {
    std::vector<std::string> args;
    for (size_t i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    if (argc == 1) {
        WriteHelp();
        return;
    }
    if (argc < 3) {
        throw std::runtime_error("Missing operand");
    }
    std::string input_file = argv[1];
    std::string output_file = argv[2];
    auto image = BMPReader::Read(input_file);
    std::unique_ptr<const filters::Filter> filter;
    for (size_t i = 3; i < argc; ++i) {
        if (args[i] == "-crop") {
            if (i + 1 == argc) {
                throw std::runtime_error("Missing operand");
            }
            const int32_t width = std::stoi(args[++i]);
            if (width <= 0) {
                throw std::runtime_error("Crop get wrong width");
            }
            if (i + 1 == argc) {
                throw std::runtime_error("Missing operand");
            }
            const int32_t height = std::stoi(args[++i]);
            if (height <= 0) {
                throw std::runtime_error("Crop get wrong height");
            }
            filter = std::make_unique<const filters::Crop>(width, height);
        } else if (args[i] == "-gs") {
            filter = std::make_unique<const filters::Grayscale>();
        } else if (args[i] == "-neg") {
            filter = std::make_unique<const filters::Negative>();
        } else if (args[i] == "-sharp") {
            filter = std::make_unique<const filters::Sharpening>();
        } else if (args[i] == "-edge") {
            if (i + 1 == argc) {
                throw std::runtime_error("Missing operand");
            }
            const float threshold = std::stof(args[++i]);
            filter = std::make_unique<const filters::EdgeDetection>(threshold);
        } else if (args[i] == "-blur") {
            if (i + 1 == argc) {
                throw std::runtime_error("Missing operand");
            }
            const float sigma = std::stof(args[++i]);
            filter = std::make_unique<const filters::GaussBlur>(sigma);
        } else if (args[i] == "-crystal") {
            filter = std::make_unique<const filters::Crystallize>();
        } else {
            throw std::runtime_error("Wrong filter operand");
        }
    }
    if (filter) {
        image = (*filter)(image);
    }
    BMPWriter::Write(output_file, image);
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
