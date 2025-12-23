#ifndef DEF_IMAGE_IO
#define DEF_IMAGE_IO

#include "Image.hpp"

class ImageIO {
    private:
    static Image loadPNG(const char* inputFilePath);
    static int mapPixelConfigurationToPNGColorType(const PixelConfiguration pixelConfiguration);

    public:
    static Image load(const char* inputFilePath);
    static void save(Image image, const char* outputFile);
};

#endif