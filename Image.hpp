#ifndef DEF_CARRE
#define DEF_CARRE

#include <string>
#include <png.h>

class Image {

    public:
    Image(const char* inputFile);
    ~Image();
    void load();
    void displayInfo();
    void convertToGrayScale();
    void saveImage(const char* outputFile);

    private:
    const char* inputFile;
    FILE* fpIn;
    png_structp pngIn;
    png_infop infoIn;

    bool isValidPng(FILE* fpIn);
};

#endif