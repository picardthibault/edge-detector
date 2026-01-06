#ifndef DEF_IMAGE
#define DEF_IMAGE

#include <string>
#include <cstdint>
#include <vector>

class ColorType {
    public: 
        enum Type { RGBA, GRAY };

        explicit ColorType(Type type);
        ColorType(const ColorType&) = default;
        ColorType& operator=(const ColorType&) = default;

        const Type getType() const;
        const int getNumberOfChannel() const;

    private:
        Type type;
};

class PixelConfiguration {
    public:
        explicit PixelConfiguration(ColorType colorType, int bitsPerChannel);
        PixelConfiguration(const PixelConfiguration&) = default;
        PixelConfiguration& operator=(const PixelConfiguration&) = default;

        const ColorType getColorType() const;
        const int getBytesPerPixel() const;
        const int getBitsPerChannel() const;

    private:
        ColorType colorType;
        int bitsPerChannel;
};

class Image {
    public:
        explicit Image(const std::uint32_t width,
            const std::uint32_t height,
            PixelConfiguration pixelConfiguration,
            std::vector<unsigned char>& data);
        Image(const Image&) = default;
        Image& operator=(const Image&) = default;

        const std::uint32_t getWidth() const;
        const std::uint32_t getHeight() const;
        const PixelConfiguration getPixelConfiguration() const;
        void setPixelConfiguration(PixelConfiguration pixelConfiguration);
        int getBytesPerRow() const;
        std::vector<unsigned char *> getRowPointers();
        std::vector<unsigned char *> getPixelPointers();
        std::vector<std::vector<unsigned char *>> getDataPointers();
        void setData(std::vector<unsigned char>& data);

    private:
        const std::uint32_t width;
        const std::uint32_t height;
        PixelConfiguration pixelConfiguration;
        std::vector<unsigned char> data;
};

#endif