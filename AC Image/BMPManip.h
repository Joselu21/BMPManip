#ifndef BMP_MANIPULATION

#define BMP_MANIPULATION

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>

constexpr uint32_t BMP_FILE_HEADER_SIZE = 14;
constexpr uint32_t BMP_INFO_HEADER_SIZE = 40;

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

struct Image {

    FileHeader fileHeader;
    InfoHeader infoHeader;
    int Width;
    int Height;

    std::vector<char> RedComponent;
    std::vector<char> GreenComponent;
    std::vector<char> BlueComponent;

    Image(int w, int h) {
        this->fileHeader = FileHeader(w, h);
        this->infoHeader = InfoHeader(w, h);
        this->Width = w;
        this->Height = h;
        this->RedComponent = std::vector<char>();
        this->GreenComponent = std::vector<char>();
        this->BlueComponent = std::vector<char>();
    }

    char RetrieveValue(char component, int x, int y) {

        int pos = x * this->Width + y;

        if (component == 'R') {
            return this->RedComponent[pos];
        }
        else if (component == 'G') {
            return this->GreenComponent[pos];
        }
        else {
            return this->BlueComponent[pos];
        }

    }

    void AssignValue(char component, int x, int y, char value) {

        if (component == 'R') {
            this->RedComponent[x * this->Width + y] = value;
        }
        else if (component == 'G') {
            this->GreenComponent[x * this->Width + y] = value;
        }
        else {
            this->BlueComponent[x * this->Width + y] = value;
        }

    }

    static Image ReadBMP(const std::string& filePath) {

        std::ifstream bmp(filePath, std::ios::binary);

        if (bmp.fail()) {
            throw std::string("File " + filePath + " cannot be opened to read.");
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

    void WriteBMP(const std::string& fileName) {

        std::ofstream bmp(fileName, std::ios::binary);

        if (bmp.fail()) {
            throw std::string("File " + fileName + " cannot be opened to write.");
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