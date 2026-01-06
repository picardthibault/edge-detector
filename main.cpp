#include <iostream>

#include "ImageIO.hpp"
#include "Processor.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage : " << argv[0] << " input.png output.png" << std::endl;
        return 1;
    }

    try {
        Image image = ImageIO::load(argv[1]);
        
        GrayScaleProcessor grayScaleProcessor = GrayScaleProcessor(image);
        grayScaleProcessor.process();

        LocalThresholdingImageProcessor globalThresholdingImageProcessor = LocalThresholdingImageProcessor(image, 27, 128, 0.1);
        Image updatedImage = globalThresholdingImageProcessor.process();
        
        ImageIO::save(updatedImage, argv[2]);
    } catch(const std::exception& e) {
        std::cerr << "Error : " << e.what() << '\n';
        return 1;
    }

    return 0;
}
