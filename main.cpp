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

        GlobalThresholdingImageProcessor globalThresholdingImageProcessor = GlobalThresholdingImageProcessor(image);
        globalThresholdingImageProcessor.process();
        
        ImageIO::save(image, argv[2]);
    } catch(const std::exception& e) {
        std::cerr << "Error : " << e.what() << '\n';
        return 1;
    }

    return 0;
}
