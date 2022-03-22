#pragma once

#include <cstddef>

class ArgsHandler {
public:
    static void Handle(size_t argc, char* argv[]);
    static void WriteHelp();
};
