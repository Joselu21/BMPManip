#ifndef BMP_FILE_HEADER

#define BMP_FILE_HEADER
#define BMP_FILE_HEADER_SIZE 14
#define BMP_INFO_HEADER_SIZE 40

#include <iostream>
#include <fstream>

/**
 * @class FileHeader
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
class FileHeader {

    private:

    public:

        char SignatureBytes[2] = { 'B', 'M' };
        uint32_t FileSize = BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE;
        uint32_t ReservedBytes = 0;
        uint32_t DataOffset = BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE;

        FileHeader() {};

        FileHeader(int w, int h) {
            this->FileSize = BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE + (uint32_t)(w * h * 3);
        }

        void Write(std::ofstream& bmp) const {
            bmp.write((char*)&this->SignatureBytes, 2);
            bmp.write((char*)&this->FileSize, 4);
            bmp.write((char*)&this->ReservedBytes, 4);
            bmp.write((char*)&this->DataOffset, 4);
        }

};

#endif