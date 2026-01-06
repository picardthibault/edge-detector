#include <iostream>
#include <algorithm>
#include <functional>
#include <cmath>
#include "Processor.hpp"
#include "ImageIO.hpp"

GrayScaleProcessor::GrayScaleProcessor(Image& image): Processor(), image(image) {
    if (image.getPixelConfiguration().getColorType().getType() != ColorType::RGBA) {
        throw std::invalid_argument("Invalid image, only RGBA image can be processed");
    }
}

GrayScaleProcessor::~GrayScaleProcessor() {}

Image GrayScaleProcessor::process() {
    std::cout << "Processing image to gray scale" << std::endl;

    PixelConfiguration newImagePixelConfiguration = PixelConfiguration(ColorType(ColorType::GRAY), 8);
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

GlobalThresholdingProcessor::GlobalThresholdingProcessor(Image& image): Processor(), image(image) {
    if (image.getPixelConfiguration().getColorType().getType() != ColorType::GRAY) {
        throw std::invalid_argument("Invalid image, only GRAY image can be processed");
    }
}

GlobalThresholdingProcessor::~GlobalThresholdingProcessor() {}

Image GlobalThresholdingProcessor::process() {
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

unsigned char* GlobalThresholdingProcessor::processPixel(unsigned char *pixelPointeur) {
    unsigned char pixelValue = *pixelPointeur;
    
    if (pixelValue > 125) {
        *pixelPointeur = 255;
    } else {
        *pixelPointeur = 0;
    }

    return pixelPointeur;
}

LocalThresholdingImageProcessor::LocalThresholdingImageProcessor(Image& image, int window, int R, float k): Processor(), image(image), window(window), R(R), k(k) {
    if (image.getPixelConfiguration().getColorType().getType() != ColorType::GRAY) {
        throw std::invalid_argument("Invalid image, only GRAY image can be processed");
    }
}

LocalThresholdingImageProcessor::~LocalThresholdingImageProcessor() {}

Image LocalThresholdingImageProcessor::process() {
    std::cout << "Processing local thresholding algorithm on image" << std::endl;

    Image resultImage = Image(this->image);
    resultImage.setPixelConfiguration(PixelConfiguration(
        ColorType(ColorType::Type::GRAY),
        8 
    ));

    std::vector<std::vector<unsigned char *>> inputImage = this->image.getDataPointers();
    std::vector<std::vector<unsigned char *>> outputImage = resultImage.getDataPointers();

    // left = aboval and right = below because our window is square
    int leftWindowRange = std::floor((this->window + 1) / 2);
    int rightWindowRange = std::floor(this->window / 2);
    int abovalWindowRange = leftWindowRange;
    int belowWindowRange = rightWindowRange;

    std::vector<uint16_t> integral = std::vector<uint16_t>(this->image.getWidth());
    std::vector<uint32_t> integralSquared = std::vector<uint32_t>(this->image.getWidth());

    for (int j = 0; j < this->image.getWidth(); j++) {
        integral.at(j) = 0;
        integralSquared.at(j) = 0;
        for (int i = 0; i < belowWindowRange; i++) {
            unsigned char pixel = *inputImage.at(i).at(j);
            integral.at(j) = integral.at(j) + pixel;
            integralSquared.at(j) = integralSquared.at(j) + (pixel * pixel);
        }
    }

    for (int i = 0; i < this->image.getHeight(); i++) {
        for (int j = 0; j < this->image.getWidth(); j++) {
            unsigned char topPixelToRemove = i - abovalWindowRange < 0 ? 0 : *inputImage.at(i - abovalWindowRange).at(j);
            unsigned char bottomPixelToAdd = i + belowWindowRange > this->image.getHeight() - 1 ? 0 : *inputImage.at(i + belowWindowRange).at(j);
            
            integral.at(j) = integral.at(j) + bottomPixelToAdd - topPixelToRemove;
            integralSquared.at(j) = integralSquared.at(j) + (bottomPixelToAdd * bottomPixelToAdd) - (topPixelToRemove * topPixelToRemove);
        }

        uint32_t windowPixelSum = 0;
        uint32_t windowPixelSumSquare = 0;

        for (int j = 0; j < rightWindowRange; j++) {
            windowPixelSum = windowPixelSum + integral.at(j);
            windowPixelSumSquare = windowPixelSumSquare + integralSquared.at(j);
        }

        for (int j = 0; j < this->image.getWidth(); j++) {
            int windowTopIndex = std::max(i - abovalWindowRange, -1);
            int windowBottomIndex = std::min(uint32_t(i + belowWindowRange), this->image.getHeight() - 1);
            int windowLeftIndex = std::max(j - leftWindowRange, -1);
            int windowRightIndex = std::min(uint32_t(j + rightWindowRange), this->image.getWidth() - 1);
            int windowPixelCount = (windowBottomIndex - windowTopIndex) * (windowRightIndex - windowLeftIndex);
            
            uint32_t windowPixelSumValueToAdd = j + rightWindowRange > this->image.getWidth() - 1 ? 0 : integral.at(j + rightWindowRange);
            uint32_t windowPixelSumValueToRemove = j - leftWindowRange < 0 ? 0 : integral.at(j - leftWindowRange);
            windowPixelSum = windowPixelSum + windowPixelSumValueToAdd - windowPixelSumValueToRemove;

            double windowMeanPixelValue = windowPixelSum / windowPixelCount;

            uint32_t windowPixelSumSquareValueToAdd = j + rightWindowRange > this->image.getWidth() - 1 ? 0 : integralSquared.at(j + rightWindowRange);
            uint32_t windowPixelSumSquareValueToRemove = j - leftWindowRange < 0 ? 0 : integralSquared.at(j - leftWindowRange);
            windowPixelSumSquare = windowPixelSumSquare + windowPixelSumSquareValueToAdd - windowPixelSumSquareValueToRemove;
       
            double variance = (windowPixelSumSquare / windowPixelCount) - (windowMeanPixelValue * windowMeanPixelValue);

            double threshold = windowMeanPixelValue * (1 + (this->k * ((std::sqrt(variance)/this->R) - 1)));

            if (*inputImage.at(i).at(j) <= threshold) {
                *outputImage.at(i).at(j) = 0;
            } else {
                *outputImage.at(i).at(j) = 255;
            }
        }
    }

    return resultImage;
}
