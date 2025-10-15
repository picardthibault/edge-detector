#include <iostream>
#include <algorithm>
#include "Processor.hpp"

GrayScaleProcessor::GrayScaleProcessor(): Processor() {}

GrayScaleProcessor::~GrayScaleProcessor() {}

void GrayScaleProcessor::process(Image& image) {
    std::cout << "Process image to gray scale" << std::endl;

    std::vector<unsigned char *> rowPointers = image.getRowPointers();
    for (int y = 0; y < image.getHeight(); y++) {
        processRow(rowPointers[y], image);
    }

    std::cout << "Successfully processed image to gray scale" << std::endl;
}

void GrayScaleProcessor::processRow(unsigned char *start, const Image& image) {
    int channel = image.getPixelConfiguration().getChannelPerPixel();
    int bytePerChannel = image.getPixelConfiguration().getBitsPerChannel() / 8;

    int i = 0;
    while (i < image.getBytesPerRow()) {
        unsigned char r = *(start + i);
        unsigned char g = *(start + i + 1);
        unsigned char b = *(start + i + 2);
        unsigned char a = *(start + i + 3);

        unsigned char grayScaleValue = (r + g + b) / 3;

        for (int y = 0; y < channel; y++) {
            *(start + i + y) = grayScaleValue;
        }
        i = i + channel * bytePerChannel;
    }
} 