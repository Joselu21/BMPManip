#ifndef BMP_MANIPULATION

#define BMP_MANIPULATION

#include <iostream>
#include <fstream>
#include <array>
#include <vector>

constexpr uint32_t BMP_FILE_HEADER_SIZE = 14;
constexpr uint32_t BMP_INFO_HEADER_SIZE = 40;

/**
 * @struct FileHeader 
 * @brief Struct type that represents the information about the file
 * 
 * This header's size is 14 bytes and the members are written in the code in the order to be written. 
 * Also, their types are defined with more explicit datatype's size to express better the size of each one inside the header.
 * 
 * @var FileHeader::SignatureBytes
 * 2 Bytes that represents the type of BMP File to be OS Compliant. By default is setted to BM to maximum compatibility
 * 
 * @var FileHeader::FileSize
 * 4 Bytes used to express the total file size. Its value by default is the sum of FileHeader and InfoHeader's 
 * sizes but it's overwritten in the constructor.
 * 
 * @var FileHeader::ReservedBytes
 * 4 bytes reserved. Some programs crash if these are different from 0.
 * 
 * @var FileHeader::DataOffset
 * 4 bytes used to determine the address where pixel data starts. By default is the sum of FileHeader and InfoHeader's sizes.
 * 
*/
struct FileHeader {

    char SignatureBytes[2] = { 'B', 'M' };
    uint32_t FileSize = BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE;
    uint32_t ReservedBytes = 0;
    uint32_t DataOffset = BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE;

    FileHeader() {};

    FileHeader(int w, int h) {
        this->FileSize = BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE + (uint32_t)(w * h * 3);
    }

    void Write(std::ofstream& bmp) {
        bmp.write((char*) &this->SignatureBytes, 2);
        bmp.write((char*) &this->FileSize, 4);
        bmp.write((char*) &this->ReservedBytes, 4);
        bmp.write((char*) &this->DataOffset, 4);
    }

};

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
struct InfoHeader {

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

    void Write(std::ofstream& bmp) {

        bmp.write((char*) &this->InfoHeaderSize, 4);
        bmp.write((char*) &this->Width, 4);
        bmp.write((char*) &this->Height, 4);
        bmp.write((char*) &this->ColorPlanes, 2);
        bmp.write((char*) &this->ColorDepth, 2);
        bmp.write((char*) &this->CompressionMethod, 4);
        bmp.write((char*) &this->RawBitmapDataSize, 4);
        bmp.write((char*) &this->HorizontalResolution, 4);
        bmp.write((char*) &this->VerticalResolution, 4);
        bmp.write((char*) &this->ColorTableEntries, 4);
        bmp.write((char*) &this->ImportantColors, 4);

    }

};


/**
 * @struct Image 
 * @brief Represents a real BMPv1 image, with all headers and pixel data.
 * 
 * All pixel data is read and stored in a channel-specific vector and MUST be accessed using the RetrieveValue function.
 * 
 * @var Image::fileHeader
 * A Header that represents the information about the file.
 * 
 * @var Image::infoHeader
 * A Header representing the information about the bitmap content of the image.
 * 
 * @var Image::Width
 * The image's width in pixels.
 * 
 * @var Image::Height
 * The image's height in pixels.
 * 
 * @var Image::RedComponent
 * A vector that stores all values from the Red component of the image
 * 
 * @var Image::GreenComponent
 * A vector that stores all values from the Green component of the image
 * 
 * @var Image::BlueComponent
 * A vector that stores all values from the Blue component of the image
 * 
*/
struct Image {

    FileHeader fileHeader;
    InfoHeader infoHeader;
    int Width;
    int Height;

    std::vector<char> RedComponent;
    std::vector<char> GreenComponent;
    std::vector<char> BlueComponent;

    /**
     * @brief Constructor of the Image struct. Initializes the headers and the pixel data containers.
     * @param w Width of the image
     * @param h Height of the image
    */
    Image(int w, int h) {
        this->fileHeader = FileHeader(w, h);
        this->infoHeader = InfoHeader(w, h);
        this->Width = w;
        this->Height = h;
        this->RedComponent = std::vector<char>();
        this->GreenComponent = std::vector<char>();
        this->BlueComponent = std::vector<char>();
    }

    /**
     * @brief Method used to read values from the images given a component and coordinates.
     * @param component Char that can be one of the following RGB, representing the color channel.
     * @param x The row of the image matrix.
     * @param y The column of the image matrix.
     * @return A Char with specific value per component, row and column.
    */

    char RetrieveValue(char component, int x, int y) const {

        int x1 = x * this->Width;
        int y1 = this->Height - y - 1;
        int pos = x1 + y1;

        if (component == 'R') {
            return this->RedComponent[pos];
        }
        else if (component == 'G') {
            return this->GreenComponent[pos];
        }
        else if (component == 'B'){
            return this->BlueComponent[pos];
        }
        else {
            throw new std::runtime_error("Invalid channel color especification in call to function RetrieveValue(" + component + ')');
        }

    }

    /**
     * @brief Method used to change the value of a specific pixel in a specific or all channels
     * @param component Char that can be one of the following RGB, representing the color channel the first three ones and all the las one.
     * @param x The row of the image matrix.
     * @param y The column of the image matrix.
     * @param value The value to be assigned at the given component and coordinates.
    */
    void AssignValue(char component, int x, int y, char value) {

        int x1 = x * this->Width;
        int y1 = this->Height - y -1;
        int pos = x1 + y1;

        if (component == 'R') {
            this->RedComponent[pos] = value;
        }
        else if (component == 'G') {
            this->GreenComponent[pos] = value;
        }
        else if (component == 'B') {
            this->BlueComponent[pos] = value;
        }
        else if (component == 'A'){
            AssignValue('R', x, y, value);
            AssignValue('G', x, y, value);
            AssignValue('B', x, y, value);
        }
        else {
            throw std::runtime_error("Invalid channel color especification in call to function AssignValue(" + component + ')');
        }

    }

    /**
     * @brief Static method to read a BMP file and return an Image object storing all the information.
     * @param filePath The path to the file to be read.
     * @return Image object that contains all the information of the given image path.
    */
    static Image ReadBMP(const std::string& filePath) {

        std::ifstream bmp(filePath, std::ios::binary);

        if (bmp.fail()) {
            throw std::runtime_error("File " + filePath + " cannot be opened to read.");
        }

        std::array<char, BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE> Header;
        bmp.read(Header.data(), Header.size());

        unsigned int Width = *reinterpret_cast<uint32_t*>(&Header[18]);
        unsigned int Height = *reinterpret_cast<uint32_t*>(&Header[22]);
        unsigned int DataSize = Width * Height * 3;

        std::vector<char> img(DataSize);
        bmp.read(img.data(), img.size());
        bmp.close();

        Image image = Image(Width, Height);

        for (int i = DataSize - 3; i >= 0; i -= 3) {

            image.BlueComponent.push_back((img[i] & 0xff));
            image.GreenComponent.push_back((img[i + 1] & 0xff));
            image.RedComponent.push_back((img[i + 2] & 0xff));

        }

        return image;
    }

    /**
     * @brief Method used to store in the disk the object that calls it, in BMPv1 format. 
     * @param fileName Path and name of the .bmp to be created.
    */
    void WriteBMP(const std::string& fileName) {

        std::ofstream bmp(fileName, std::ios::binary);

        if (bmp.fail()) {
            throw std::runtime_error("File " + fileName + " cannot be opened to write.");
        }

        // File Header
        this->fileHeader.Write(bmp);
                      
        // Info Header
        this->infoHeader.Write(bmp);

        // Pixel Write 
        for (int h = this->Height - 1; h >= 0; h--) {

            int beginFactor = h * this->Width;
            int endFactor = beginFactor + this->Width;

            std::vector<char> r(this->RedComponent.begin() + beginFactor, this->RedComponent.begin() + endFactor);
            std::vector<char> g(this->GreenComponent.begin() + beginFactor, this->GreenComponent.begin() + endFactor);
            std::vector<char> b(this->BlueComponent.begin() + beginFactor,this->BlueComponent.begin() + endFactor);

            for (int i = r.size() - 1; i >= 0; i--) {

                bmp.write((char*)&b[i], 1);
                bmp.write((char*)&g[i], 1);
                bmp.write((char*)&r[i], 1);

            }

        }

        bmp.close();
    }

};

#endif // !BMP_MANIPULATION