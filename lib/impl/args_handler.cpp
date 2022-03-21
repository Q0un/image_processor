#include "args_handler.h"

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
    for (size_t i = 3; i < argc; ++i) {
        if (std::strcmp(argv[i], "-crop") == 0) {

        }
    }
}
