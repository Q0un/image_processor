#include <catch.hpp>

#include "args_handler.h"

#include <cstring>
#include <string>
#include <vector>

void StringsToCstrings(std::vector<std::string> strings, char* cstrings[]) {
    for (size_t i = 0; i < strings.size(); ++i) {
        cstrings[i] = new char[strings[i].length() + 1];
        std::strcpy(cstrings[i], strings[i].c_str());
    }
}

void DeleteCStrings(size_t argv, char* cstrings[]) {
    for (size_t i = 0; i < argv; ++i) {
        delete[] cstrings[i];
    }
}

TEST_CASE("Test help") {
    const size_t argv = 1;
    char* ar[argv];
    std::vector<std::string> v = {"image_processor"};
    StringsToCstrings(v, ar);
    REQUIRE_NOTHROW(ArgsHandler::Handle(argv, ar));
    DeleteCStrings(argv, ar);
}

TEST_CASE("Test not output failing") {
    const size_t argv = 2;
    char* ar[argv];
    std::vector<std::string> v = {"image_processor", "kek"};
    StringsToCstrings(v, ar);
    REQUIRE_THROWS_WITH(ArgsHandler::Handle(argv, ar), "Missing operand");
    DeleteCStrings(argv, ar);
}

TEST_CASE("Test without filters") {
    const size_t argv = 3;
    char* ar[argv];
    std::vector<std::string> v = {"image_processor", EXAMPLE_PATH,
                                  "/tmp/out.bmp"};
    StringsToCstrings(v, ar);
    REQUIRE_NOTHROW(ArgsHandler::Handle(argv, ar));
    DeleteCStrings(argv, ar);
}

TEST_CASE("Test good filters") {
    const size_t argv = 8;
    char* ar[argv];
    std::vector<std::string> v = {"image_processor", EXAMPLE_PATH,
                                  "/tmp/out.bmp", "-neg", "-crop", "1", "1",
                                  "-gs"};
    StringsToCstrings(v, ar);
    REQUIRE_NOTHROW(ArgsHandler::Handle(argv, ar));
    DeleteCStrings(argv, ar);
}

TEST_CASE("Test wrong filter") {
    const size_t argv = 4;
    char* ar[argv];
    std::vector<std::string> v = {"image_processor", EXAMPLE_PATH,
                                  "/tmp/out.bmp", "-kek"};
    StringsToCstrings(v, ar);
    REQUIRE_THROWS_WITH(ArgsHandler::Handle(argv, ar), "Wrong filter operand");
    DeleteCStrings(argv, ar);
}
