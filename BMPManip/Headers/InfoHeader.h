#ifndef BMP_INFO_HEADER

#define BMP_INFO_HEADER
#define BMP_FILE_HEADER_SIZE 14
#define BMP_INFO_HEADER_SIZE 40

#include <iostream>
#include <fstream>

/**
 * @struct InfoHeader
 * @brief Struct type that represents the information about the BMP type. This struct represent a BMPv1 DIB Header
 *
 * This header's size is 40 bytes and the members are written in the code in the order to be written.
 * Also, their types are defined with more explicit datatype's size to express better the size of each one inside the header.
 *
 * @var InfoHeader::InfoHeaderSize
 * Size of this header. Is often used to determine the Bitmap version. The default value for BMPv1 is 40 bytes
 *
 * @var InfoHeader::Width
 * Width in pixels
 *
 * @var InfoHeader::Height
 * Height in pixels
 *
 * @var InfoHeader::ColorPlanes
 * Number of color planes. Must be set to 1.
 *
 * @var InfoHeader::ColorDepth
 * Number of bits per pixel. Currently is only supported the 24-bit depth, with 8bits per color channel in RGB.
 *
 * @var InfoHeader::CompressionMethod
 * The compression method used in the pixel data. Currently is only supported the default value, 0, which stands for no compression.
 *
 * @var InfoHeader::RawBitmapDataSize
 * The size of the pixel data. It can be 0 if no compression method is given, as in this case.
 *
 * @var InfoHeader::HorizontalResolution
 * The horizontal pixels per metre. It can be any feasible number.
 *
 * @var InfoHeader::VerticalResolution
 * The vertical pixels per metre. It can be any feasible number.
 *
 * @var InfoHeader::ColorTableEntries
 * Number of colors in the palette. Its not important if you work with 24-bit color depth because there is no color tables.
 *
 * @var InfoHeader::ImportantColors
 * Deprecated, it represents the most important colors of the BMP.
*/
class InfoHeader {

private:

public:

    uint32_t InfoHeaderSize = BMP_INFO_HEADER_SIZE;
    int32_t Width;
    int32_t Height;
    uint16_t ColorPlanes = 1;
    uint16_t ColorDepth = 24;
    uint32_t CompressionMethod = 0;
    uint32_t RawBitmapDataSize = 0;
    int32_t HorizontalResolution;
    int32_t VerticalResolution;
    uint32_t ColorTableEntries = 0;
    uint32_t ImportantColors = 0;

    InfoHeader() {
        this->Width = 32;
        this->Height = 32;
        this->HorizontalResolution = 32;
        this->VerticalResolution = 32;
    };

    InfoHeader(int w, int h) {
        this->Width = w;
        this->Height = h;
        this->HorizontalResolution = w;
        this->VerticalResolution = h;
        this->RawBitmapDataSize = w * h * 3;
    }

    void Write(std::ofstream& bmp) const {

        bmp.write((char*)&this->InfoHeaderSize, 4);
        bmp.write((char*)&this->Width, 4);
        bmp.write((char*)&this->Height, 4);
        bmp.write((char*)&this->ColorPlanes, 2);
        bmp.write((char*)&this->ColorDepth, 2);
        bmp.write((char*)&this->CompressionMethod, 4);
        bmp.write((char*)&this->RawBitmapDataSize, 4);
        bmp.write((char*)&this->HorizontalResolution, 4);
        bmp.write((char*)&this->VerticalResolution, 4);
        bmp.write((char*)&this->ColorTableEntries, 4);
        bmp.write((char*)&this->ImportantColors, 4);

    }

};

#endif