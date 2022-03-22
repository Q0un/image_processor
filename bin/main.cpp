#include "image.h"
#include "filters.h"

int main(int argc, char* argv[]) {
    Image image = Image::FromFile("/home/qqun/CLionProjects/image_processor/examples/example.bmp");
    filters::GaussBlur filter(10);
    image = filter(image);
    image.SaveToFile("/home/qqun/kekw.bmp");
}
