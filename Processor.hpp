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
        explicit GrayScaleProcessor(Image& image);
        ~GrayScaleProcessor() override;
        Image process() override;
};

class GlobalThresholdingProcessor: public Processor {
    private:
        Image& image;
        unsigned char* processPixel(unsigned char *pixelPointeur);

    public:
        explicit GlobalThresholdingProcessor(Image& image);
        ~GlobalThresholdingProcessor() override;
        Image process() override;
};

class LocalThresholdingImageProcessor: public Processor {
    private:
        Image& image;
        const int window;
        const int R;
        const float k;

    public:
        explicit LocalThresholdingImageProcessor(Image& image, int window, int R, float k);
        ~LocalThresholdingImageProcessor() override;
        Image process() override;
};

#endif