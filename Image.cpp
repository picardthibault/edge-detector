#include "Image.hpp"
#include <png.h>
#include <iostream>

Image::Image(const std::string inputFilePath,
        const std::uint32_t width,
        const std::uint32_t height,
        const int channel,
        const int bitsPerChannel,
        const std::vector<unsigned char>& data
    ): inputFilePath(inputFilePath), width(width), height(height), channel(channel), bitsPerChannel(bitsPerChannel), data(data) {}

std::uint32_t Image::getWidth() const {
    return this->width;
}

std::uint32_t Image::getHeight() const {
    return this->height;
}

int Image::getChannel() const {
    return this->channel;
}

int Image::getBitsPerChannel() const {
    return this->bitsPerChannel;
}

const std::vector<unsigned char>& Image::getData() const {
    return this->data;
}

Image ImageIO::load(const char* inputFilePath) {
    std::cout << "Load image from \"" << inputFilePath << "\"" << std::endl;
    return ImageIO::loadPNG(inputFilePath);
}

Image ImageIO::loadPNG(const char* inputFilePath) {
    std::cout << "Load image from \"" << inputFilePath << "\" as PNG image" << std::endl;

    FILE* fpIn = fopen(inputFilePath, "rb");

    if (!fpIn) {
        throw std::invalid_argument(std::string("Unable to open file \"") + inputFilePath + "\".");
    }
    
    png_byte header[8];
    fread(header, 1, 8, fpIn);
    bool isValidPng = png_sig_cmp(header, 0, 8) == 0;

    if (!isValidPng) {
        fclose(fpIn);
        throw std::invalid_argument(std::string("File \"") + inputFilePath + "\" is not a valid PNG image.");
    }

    png_structp pngIn = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!pngIn) {
        png_destroy_read_struct(&pngIn, nullptr, nullptr);
        fclose(fpIn);
        throw std::runtime_error("Unable to create PNG image read structure");
    }

    png_infop infoIn = png_create_info_struct(pngIn);
    if (!infoIn) {
        png_destroy_info_struct(pngIn, &infoIn);
        png_destroy_read_struct(&pngIn, nullptr, nullptr);
        fclose(fpIn);
        throw std::runtime_error("Unable to create info structure");
    }

    if (setjmp(png_jmpbuf(pngIn))) {
        png_destroy_info_struct(pngIn, &infoIn);
        png_destroy_read_struct(&pngIn, nullptr, nullptr);
        fclose(fpIn);
        throw std::ios_base::failure("Fail to load PNG image.");
    }

    png_init_io(pngIn, fpIn);
    png_set_sig_bytes(pngIn, 8);
    png_read_info(pngIn, infoIn);

    std::uint32_t width  = png_get_image_width(pngIn, infoIn);
    std::uint32_t height = png_get_image_height(pngIn, infoIn);
    png_byte colorType = png_get_color_type(pngIn, infoIn);
    png_byte bitDepth  = png_get_bit_depth(pngIn, infoIn);

    // format image to 8bits per channel RGBA image
    if (bitDepth == 16) {
        png_set_strip_16(pngIn);
    }
    if (colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(pngIn);
    }
    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
        png_set_expand_gray_1_2_4_to_8(pngIn);
    }
    if (png_get_valid(pngIn, infoIn, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(pngIn);
    }
    if (colorType == PNG_COLOR_TYPE_RGB ||
        colorType == PNG_COLOR_TYPE_GRAY ||
        colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(pngIn, 0xFF, PNG_FILLER_AFTER);
    }
    if (colorType == PNG_COLOR_TYPE_GRAY ||
        colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(pngIn);
    }

    png_read_update_info(pngIn, infoIn);

    int channel = 4; // because RGBA normalization
    int bitsPerChannel = 8; // because 8bit per channel normalization

    size_t rowBytes = png_get_rowbytes(pngIn, infoIn);
    std::vector<unsigned char> pixels(height * rowBytes);

    std::vector<png_bytep> rowPointers(height);
    for (int y = 0; y < height; y++) {
        rowPointers[y] = pixels.data() + y * rowBytes;
    }

    png_read_image(pngIn, rowPointers.data());

    png_destroy_info_struct(pngIn, &infoIn);
    png_destroy_read_struct(&pngIn, nullptr, nullptr);
    fclose(fpIn);

    return Image(inputFilePath, width, height, channel, bitsPerChannel, pixels);
}

void ImageIO::save(Image image, const char* outputFilePath) {
    std::cout << "Save PNG image to \"" << outputFilePath << "\"" << std::endl;

    FILE* fpOut = fopen(outputFilePath, "wb");
    if (!fpOut) {
        throw std::invalid_argument(std::string("Unable to create output file \"") + outputFilePath + "\".");
    }

    png_structp pngOut = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!pngOut) {
        fclose(fpOut);
        throw std::runtime_error("Unable to create write structure");
    }
    png_infop infoOut = png_create_info_struct(pngOut);
    if (!infoOut) {
        fclose(fpOut);
        png_destroy_write_struct(&pngOut, &infoOut);
        throw std::runtime_error("Unable to create write structure");
    }

    if (setjmp(png_jmpbuf(pngOut))) {
        fclose(fpOut);
        png_destroy_write_struct(&pngOut, &infoOut);
        throw std::invalid_argument("Unable to write in output file");
    }

    png_init_io(pngOut, fpOut);
    png_set_IHDR(pngOut,
        infoOut,
        image.getWidth(),
        image.getHeight(),
        image.getBitsPerChannel(),
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);
    png_write_info(pngOut, infoOut);

    const std::vector<unsigned char>& dataRef = image.getData();
    std::vector<png_bytep> rowPointers(image.getHeight());
    size_t rowbytes = image.getWidth() * image.getChannel() * (image.getBitsPerChannel() / 8);
    for (size_t y = 0; y < image.getHeight(); y++) {
        rowPointers[y] = (png_bytep)(dataRef.data() + y * rowbytes);
    }

    png_write_image(pngOut, rowPointers.data());
    png_write_end(pngOut, nullptr);

    fclose(fpOut);
    png_destroy_write_struct(&pngOut, &infoOut);
}
