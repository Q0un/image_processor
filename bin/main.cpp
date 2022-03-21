#include "image.h"

int main(int argc, char* argv[]) {
    Image image = Image::FromFile("/home/qqun/CLionProjects/image_processor/examples/example.bmp");
    image.SaveToFile("/home/qqun/kekw.bmp");
}