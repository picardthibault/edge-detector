#ifndef DEF_PROCESSOR
#define DEF_PROCESSOR

#include "Image.hpp"

class Processor {
    public: 
        virtual void process(Image& image) = 0;
        virtual ~Processor() {}
};

class GrayScaleProcessor: public Processor {

    private:
        void processRow(unsigned char *start, const Image& image);

    public:
        GrayScaleProcessor();
        ~GrayScaleProcessor() override;
        void process(Image& image) override;
};

#endif