#ifndef BMP_IMAGE

#define BMP_IMAGE

#include "FileHeader.h"
#include "InfoHeader.h"
#include <iostream>
#include <fstream>
#include <array>
#include <vector>

/**
 * @class Image 
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
 * A vector that stores all values from the Red component of the image.
 * 
 * @var Image::GreenComponent
 * A vector that stores all values from the Green component of the image.
 * 
 * @var Image::BlueComponent
 * A vector that stores all values from the Blue component of the image.
 * 
 * @var Image::GreyComponent
 * A vector that stores greyscale values of the color image.
 * 
*/
class Image {

    private:

        FileHeader fileHeader;
        InfoHeader infoHeader;

    public:

        std::vector<unsigned char> RedComponent;
        std::vector<unsigned char> GreenComponent;
        std::vector<unsigned char> BlueComponent;

        int Width;
        int Height;

        /**
         * @brief Constructor of the Image class. Initializes the headers and the pixel data containers.
         * @param w Width of the image.
         * @param h Height of the image.
        */
        Image(int w, int h);

        /**
         * @brief Constructor of the Image class with array of data from a grey image and the size
         * @param Grey Array of pixel data
         * @param Width Width of the image.
         * @param Height Height of the image.
        */
        Image(unsigned char* Grey, int Width, int Height);

        /**
         * @brief Method used to read values from the images given a component and coordinates.
         * @param component Char that can be one of the following RGB, representing the color channel.
         * @param x The row of the image matrix.
         * @param y The column of the image matrix.
         * @return A Char with specific value per component, row and column.
        */
        char RetrieveValue(char component, int x, int y) const;

        /**
         * @brief Method used to change the value of a specific pixel in a specific or all channels
         * @param component Char that can be one of the following RGB, representing the color channel the first three ones and all the las one.
         * @param x The row of the image matrix.
         * @param y The column of the image matrix.
         * @param value The value to be assigned at the given component and coordinates.
        */
        void AssignValue(char component, int x, int y, unsigned char value);

        /**
         * @brief Method used to store in the disk the object that calls it in RGB mode, in BMPv1 format. 
         * @param fileName Path and name of the .bmp to be created.
        */
        void WriteBMP(const std::string& filePath) const;

        /**
         * @brief Method that converts a single greyscaled image to a primitive array.
         * @return Array with the pixel data.
        */
        unsigned char* ToArray() const;

        /**
         * @brief Method that converts a single greyscaled image to a primitive matrix.
         * @return Matrix with ordered pixel data.
        */
        unsigned char** ToMatrix() const;

        /**
         * @brief Method that changed instance information stored by the values given.
         * @param Img Array with the pixel data.
         * @param Width Width of the image.
         * @param Height Height of the image.
        */
        void FromArray(unsigned char* Img, int Width, int Height);


        /**
         * @brief Coordinates adapter to the in-class specific order or the BMP pixel store order.
         * @param x X Coord.
         * @param y Y Coord.
         * @return Integer that represents the position in the in-class array.
        */
        int AdaptCoords(int x, int y);

        /**
         * @brief Static method to read a BMP file and return an Image object storing all the information.
         * @param filePath The path to the file to be read.
         * @return Image object that contains all the information of the given image path.
        */
        static Image ReadBMP(const std::string& filePath);

        /**
         * @brief Method used to populate the GreyComponent vector.
         * @param Img Image class to be adapted to grayscale.
         * @return The Image object grayscaled.
        */
        static Image AdaptToGrayScale(const Image& Img);

        /**
         * @brief Coordinates adapter to the in-class specific order or the BMP pixel store order.
         * @param x X Coord.
         * @param y Y Coord.
         * @param w Width of the image.
         * @param h Height of the image.
         * @return Integer that represents the position in the in-class array.
        */
        static int AdaptCoords(int x, int y, int Width, int Height);

};

#endif // !BMP_MANIPULATION