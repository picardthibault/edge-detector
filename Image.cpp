#include <iostream>

#include "Image.hpp"

ColorType::ColorType(ColorType::Type type): type(type) {}

const ColorType::Type ColorType::getType() const {
    return type;
}

const int ColorType::getNumberOfChannel() const {
    switch (type) {
        case RGBA :
            return 4;
        case GRAY :
            return 1;
        default:
            throw std::invalid_argument("Unknown color type");
    }
}

PixelConfiguration::PixelConfiguration(const ColorType colorType, const int bitsPerChannel): colorType(colorType), bitsPerChannel(bitsPerChannel) {}

const ColorType PixelConfiguration::getColorType() const {
    return colorType;
}

const int PixelConfiguration::getBytesPerPixel() const {
    return colorType.getNumberOfChannel() * bitsPerChannel / 8;
}

const int PixelConfiguration::getBitsPerChannel() const {
    return bitsPerChannel;
}

Image::Image(const std::uint32_t width,
        const std::uint32_t height,
        PixelConfiguration pixelConfiguration,
        std::vector<unsigned char>& data
    ): width(width), height(height), pixelConfiguration(pixelConfiguration), data(data) {}

const std::uint32_t Image::getWidth() const {
    return width;
}

const std::uint32_t Image::getHeight() const {
    return height;
}

const PixelConfiguration Image::getPixelConfiguration() const {
    return pixelConfiguration;
}

void Image::setPixelConfiguration(PixelConfiguration pixelConfiguration) {
    this->pixelConfiguration = pixelConfiguration;
}

int Image::getBytesPerRow() const {
    return getWidth() * pixelConfiguration.getBytesPerPixel();
}

std::vector<unsigned char *> Image::getRowPointers() {
    int bytesPerRow = this->getBytesPerRow();

    std::vector<unsigned char *> rowPointers(height);
    for (int y = 0; y < height; y++) {
        rowPointers[y] = data.data() + y * bytesPerRow;
    }

    return rowPointers;
}

std::vector<unsigned char *> Image::getPixelPointers() {
    int numberOfPixels = height * width;
    std::vector<unsigned char *> pixelPointers(numberOfPixels);

    for (int i = 0; i < numberOfPixels; i++) {
        pixelPointers[i] = data.data() + i * pixelConfiguration.getBytesPerPixel();
    }

    return pixelPointers;
}

void Image::setData(std::vector<unsigned char>& data) {
    this->data = data;
}