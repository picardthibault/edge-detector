#ifndef DEF_IMAGE
#define DEF_IMAGE

#include <string>
#include <cstdint>
#include <vector>

class ColorType {
    public: 
        enum Type { RGBA, GRAY };

        ColorType(Type type);

        const Type getType() const;
        const int getNumberOfChannel() const;

    private:
        Type type;
};

class PixelConfiguration {
    public:
        PixelConfiguration(const ColorType colorType, const int bitsPerChannel);
        const ColorType getColorType() const;
        const int getBytesPerPixel() const;
        const int getBitsPerChannel() const;

    private:
        ColorType colorType;
        int bitsPerChannel;
};

class Image {
    public:
        Image(const std::uint32_t width,
            const std::uint32_t height,
            PixelConfiguration pixelConfiguration,
            std::vector<unsigned char>& data);

        const std::uint32_t getWidth() const;
        const std::uint32_t getHeight() const;
        const PixelConfiguration getPixelConfiguration() const;
        void setPixelConfiguration(PixelConfiguration pixelConfiguration);
        int getBytesPerRow() const;
        std::vector<unsigned char *> getRowPointers();
        std::vector<unsigned char *> getPixelPointers();
        void setData(std::vector<unsigned char>& data);

    private:
        const std::uint32_t width;
        const std::uint32_t height;
        PixelConfiguration pixelConfiguration;
        std::vector<unsigned char> data;
};

#endif