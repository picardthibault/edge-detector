#include <iostream>

#include "Image.hpp"

PixelConfiguration::PixelConfiguration(const Channel value, const int bitsPerChannel): channel(value), bitsPerChannel(bitsPerChannel) {}

const int PixelConfiguration::getChannelPerPixel() const {
    switch (channel) {
    case RGBA :
        return 4;
    case GRAY :
        return 1;
    default:
        return 0;
    }
}

const int PixelConfiguration::getBytesPerPixel() const {
    return getChannelPerPixel() * bitsPerChannel / 8;
}

const int PixelConfiguration::getBitsPerChannel() const {
    return bitsPerChannel;
}

Image::Image(const std::string inputFilePath,
        const std::uint32_t width,
        const std::uint32_t height,
        PixelConfiguration pixelConfiguration,
        std::vector<unsigned char>& data
    ): inputFilePath(inputFilePath), width(width), height(height), pixelConfiguration(pixelConfiguration), data(data) {}

const std::uint32_t Image::getWidth() const {
    return width;
}

const std::uint32_t Image::getHeight() const {
    return height;
}

const PixelConfiguration Image::getPixelConfiguration() const {
    return pixelConfiguration;
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

std::vector<unsigned char>& Image::getData() {
    return data;
}
