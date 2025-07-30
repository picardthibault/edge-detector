#include <png.h>
#include <iostream>

#include "Image.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage : " << argv[0] << " input.png output.png" << std::endl;
        return 1;
    }

    try {
        Image image = Image(argv[1]);
        image.load();
        image.convertToGrayScale();
        image.saveImage(argv[2]);
    } catch(const std::exception& e) {
        std::cerr << "Error : " << e.what() << '\n';
        return 1;
    }

    return 0;
}
