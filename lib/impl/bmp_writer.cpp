#include "bmp_writer.h"

void BMPWriter::Write(std::string_view file_name, const Image& image) {
    std::ofstream output(file_name.data(), std::ios_base::binary);
    output.write(reinterpret_cast<const char*>(&image.GetFileHeader()),
                 sizeof(BMPFileHeader));
    output.write(reinterpret_cast<const char*>(&image.GetInfoHeader()),
                 sizeof(BMPInfoHeader));
    size_t rowPadding = image.Padding();
    size_t x = image.Height() - 1;
    do {
        for (size_t y = 0; y < image.Width(); ++y) {
            Pixel pixel = image.GetPixel(x, y);
            output.put(static_cast<char>(pixel.b * 255));
            output.put(static_cast<char>(pixel.g * 255));
            output.put(static_cast<char>(pixel.r * 255));
        }
        for (size_t j = 0; j < rowPadding; ++j) {
            output.put(0);
        }
    } while (x--);
    output.close();
}
