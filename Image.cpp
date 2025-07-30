#include "Image.hpp"
#include <iostream>

Image::Image(const char* inputFile): inputFile(inputFile) {
    FILE* fpIn = fopen(inputFile, "rb");

    if (!fpIn) {
        throw std::invalid_argument(std::string("Unable to open file \"") + inputFile + "\".");
    }
    this->fpIn = fpIn;
}

Image::~Image() {
    fclose(fpIn);
    png_destroy_read_struct(&pngIn, &infoIn, nullptr);
}

void Image::load() {
    std::cout << "Load image from \"" << inputFile << "\"" << std::endl;

    if (!isValidPng(fpIn)) {
        fclose(fpIn);
        throw std::invalid_argument(std::string("File \"") + inputFile + "\" is not a valid PNG.");
    }

    pngIn = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    infoIn = png_create_info_struct(pngIn);

    if (setjmp(png_jmpbuf(pngIn))) {
        fclose(fpIn);
        throw std::ios_base::failure("Fail to load PNG image.");
    }

    png_init_io(pngIn, fpIn);
    png_set_sig_bytes(pngIn, 8);
    png_read_info(pngIn, infoIn);
}

bool Image::isValidPng(FILE* fpIn) {
    std::cout << "Check if image is a valid PNG" << std::endl;
    png_byte header[8];
    fread(header, 1, 8, fpIn);
    return png_sig_cmp(header, 0, 8) == 0;
}

void Image::displayInfo() {
    if (setjmp(png_jmpbuf(pngIn))) {
        throw std::runtime_error("Fail to display PNG image info.");
    }

    png_read_info(pngIn, infoIn);

    png_uint_32 width = png_get_image_width(pngIn, infoIn);
    png_uint_32 height = png_get_image_height(pngIn, infoIn);
    int bitDepth = png_get_bit_depth(pngIn, infoIn);
    int colorType = png_get_color_type(pngIn, infoIn);

    std::cout << "Image data :" << std::endl;
    std::cout << "  Width : " << width << " pixels" << std::endl;
    std::cout << "  Height : " << height << " pixels" << std::endl;
    std::cout << "  Bit deph : " << bitDepth << std::endl;
    std::cout << "  Color type : " << colorType << std::endl;
}

void Image::convertToGrayScale() {
    std::cout << "Convert image to gray scale" << std::endl;

    if (setjmp(png_jmpbuf(pngIn))) {
        throw std::runtime_error("Fail to convert image to gray scale");
    }

    int colorType = png_get_color_type(pngIn, infoIn);
    int bitDepth = png_get_bit_depth(pngIn, infoIn);

    if (bitDepth == 16)
        png_set_strip_16(pngIn);

    if (colorType == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(pngIn);

    if (png_get_valid(pngIn, infoIn, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(pngIn);

    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(pngIn);

    png_set_rgb_to_gray(pngIn, 1, -1.0, -1.0);

    png_read_update_info(pngIn, infoIn);
}

void Image::saveImage(const char* outputFile) {
    std::cout << "Save image to \"" << outputFile << "\"" << std::endl;

    png_uint_32 width = png_get_image_width(pngIn, infoIn);
    png_uint_32 height = png_get_image_height(pngIn, infoIn);

    int rowbytes = png_get_rowbytes(pngIn, infoIn);
    png_bytep imageData = (png_bytep)malloc(rowbytes * height);
    png_bytep* rowPointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (png_uint_32 i = 0; i < height; ++i) {
        rowPointers[i] = imageData + i * rowbytes;
    }

    png_read_image(pngIn, rowPointers);
    
    FILE* fpOut = fopen(outputFile, "wb");
    if (!fpOut) {
        free(imageData);
        free(rowPointers);
        throw std::invalid_argument(std::string("Unable to create output file \"") + outputFile + "\".");
    }

    png_structp pngOut = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop infoOut = png_create_info_struct(pngOut);
    if (setjmp(png_jmpbuf(pngOut))) {
        fclose(fpOut);
        free(imageData);
        free(rowPointers);
        png_destroy_write_struct(&pngOut, &infoOut);
        throw std::invalid_argument("Unable to write in output file");
    }

    png_init_io(pngOut, fpOut);
    png_set_IHDR(pngOut, infoOut, width, height, 8,
                 PNG_COLOR_TYPE_GRAY_ALPHA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(pngOut, infoOut);
    png_write_image(pngOut, rowPointers);
    png_write_end(pngOut, nullptr);

    fclose(fpOut);
    png_destroy_write_struct(&pngOut, &infoOut);
    free(imageData);
    free(rowPointers);
}
