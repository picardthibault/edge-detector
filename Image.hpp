#ifndef DEF_IMAGE
#define DEF_IMAGE

#include <string>
#include <cstdint>
#include <vector>

class PixelConfiguration {
    public:
        enum Channel { RGBA, GRAY };
        PixelConfiguration(const Channel type, const int bitsPerChannel);
        const int getChannelPerPixel() const;
        const int getBytesPerPixel() const;
        const int getBitsPerChannel() const;

    private:
        Channel channel;
        int bitsPerChannel;
};

class Image {
    public:
        Image(const std::string inputFilePath,
            const std::uint32_t width,
            const std::uint32_t height,
            PixelConfiguration pixelConfiguration,
            std::vector<unsigned char>& data);

        const std::uint32_t getWidth() const;
        const std::uint32_t getHeight() const;
        const PixelConfiguration getPixelConfiguration() const;
        int getBytesPerRow() const;
        std::vector<unsigned char *> getRowPointers();
        std::vector<unsigned char>& getData();

    private:
        const std::string inputFilePath;
        const std::uint32_t width;
        const std::uint32_t height;
        PixelConfiguration pixelConfiguration;
        std::vector<unsigned char> data;
};

#endif