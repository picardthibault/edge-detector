#ifndef DEF_PROCESSOR
#define DEF_PROCESSOR

#include "Image.hpp"

class Processor {
    public: 
        virtual Image process() = 0;
        virtual ~Processor() {}
};

class GrayScaleProcessor: public Processor {
    private:
        Image& image;
        unsigned char processPixel(unsigned char *pixelPointeur);

    public:
        GrayScaleProcessor(Image& image);
        ~GrayScaleProcessor() override;
        Image process() override;
};

class GlobalThresholdingImageProcessor: public Processor {
    private:
        Image& image;
        unsigned char* processPixel(unsigned char *pixelPointeur);

    public:
        GlobalThresholdingImageProcessor(Image& image);
        ~GlobalThresholdingImageProcessor() override;
        Image process() override;
};

#endif