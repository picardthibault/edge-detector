#include <iostream>
#include <algorithm>
#include <functional>
#include "Processor.hpp"

GrayScaleProcessor::GrayScaleProcessor(Image& image): Processor(), image(image) {
    if (image.getPixelConfiguration().getColorType().getType() != ColorType::RGBA) {
        throw std::invalid_argument("Invalid image, only RGBA image can be processed");
    }
}

GrayScaleProcessor::~GrayScaleProcessor() {}

Image GrayScaleProcessor::process() {
    std::cout << "Processing image to gray scale" << std::endl;

    PixelConfiguration newImagePixelConfiguration = PixelConfiguration(ColorType::GRAY, 8);
    int numberOfPixels = image.getHeight() * image.getWidth();

    std::vector<unsigned char> newImagePixels(numberOfPixels * newImagePixelConfiguration.getBytesPerPixel());

    std::vector<unsigned char *> originalImagePixelPointers = image.getPixelPointers();
    
    std::transform(originalImagePixelPointers.cbegin(),
        originalImagePixelPointers.cend(),
        newImagePixels.begin(),
        [this](unsigned char *pixel) { return this->processPixel(pixel); }
    );

    std::cout << "Successfully processed image to gray scale" << std::endl;

    image.setPixelConfiguration(newImagePixelConfiguration);
    image.setData(newImagePixels);

    return image;
}

unsigned char GrayScaleProcessor::processPixel(unsigned char *pixelPointer) {
    unsigned char r = *(pixelPointer);
    unsigned char g = *(pixelPointer + 1);
    unsigned char b = *(pixelPointer + 2);
    return (r + g + b) / 3;
}

GlobalThresholdingImageProcessor::GlobalThresholdingImageProcessor(Image& image): Processor(), image(image) {
    if (image.getPixelConfiguration().getColorType().getType() != ColorType::GRAY) {
        throw std::invalid_argument("Invalid image, only GRAY image can be processed");
    }
}

GlobalThresholdingImageProcessor::~GlobalThresholdingImageProcessor() {}

Image GlobalThresholdingImageProcessor::process() {
    std::cout << "Processing global thresholding algorithm on image" << std::endl;

    std::vector<unsigned char *> imagePixelPointers = image.getPixelPointers();
    
    std::transform(imagePixelPointers.cbegin(),
        imagePixelPointers.cend(),
        imagePixelPointers.begin(),
        [this](unsigned char *pixel) { return this->processPixel(pixel); }
    );

    std::cout << "Successfully processed global thresholding algorithm on image" << std::endl;

    return image;
}

unsigned char* GlobalThresholdingImageProcessor::processPixel(unsigned char *pixelPointeur) {
    unsigned char pixelValue = *pixelPointeur;
    
    if (pixelValue > 125) {
        *pixelPointeur = 255;
    } else {
        *pixelPointeur = 0;
    }

    return pixelPointeur;
}