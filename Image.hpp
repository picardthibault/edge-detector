#ifndef DEF_IMAGE
#define DEF_IMAGE

#include <string>
#include <cstdint>
#include <vector>

class Image {

    private:
    const std::string inputFilePath;
    const std::uint32_t width;
    const std::uint32_t height;
    const int channel;
    const int bitsPerChannel;
    const std::vector<unsigned char> data;

    public:
    Image(const std::string inputFilePath,
        const std::uint32_t width,
        const std::uint32_t height,
        const int channel,
        const int bitsPerChannel,
        const std::vector<unsigned char>& data);
    std::uint32_t getWidth() const;
    std::uint32_t getHeight() const;
    int getChannel() const;
    int getBitsPerChannel() const;
    const std::vector<unsigned char>& getData() const;
};

class ImageIO {
    private:
    static Image loadPNG(const char* inputFilePath);

    public:
    static Image load(const char* inputFilePath);
    static void save(Image image, const char* outputFile);
};

#endif