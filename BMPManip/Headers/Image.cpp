#include "Image.h"

Image::Image(int w, int h) {
    this->fileHeader = FileHeader(w, h);
    this->infoHeader = InfoHeader(w, h);
    this->Width = w;
    this->Height = h;
    this->RedComponent = std::vector<unsigned char>();
    this->GreenComponent = std::vector<unsigned char>();
    this->BlueComponent = std::vector<unsigned char>();
}

Image::Image(unsigned char* Grey, int Width, int Height) {

    this->Width = Width;
    this->Height = Height;
    // TODO

}

char Image::RetrieveValue(char component, int x, int y) const {

    if (x < 0 || x >= this->Height || y < 0 || y >= this->Width) { // TODO: Change to exception
        return -1;
    }

    int x1 = x * this->Width;
    int y1 = this->Height - y - 1;
    int pos = x1 + y1;

    if (component == 'R') {
        return this->RedComponent[pos];
    }
    else if (component == 'G') {
        return this->GreenComponent[pos];
    }
    else if (component == 'B') {
        return this->BlueComponent[pos];
    }
    else {
        throw new std::runtime_error("Invalid channel color especification in call to function RetrieveValue(" + component + ')');
    }

}

void Image::AssignValue(char component, int x, int y, unsigned char value) {

    int x1 = x * this->Width;
    int y1 = this->Height - y - 1;
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
    else if (component == 'A') {
        AssignValue('R', x, y, value);
        AssignValue('G', x, y, value);
        AssignValue('B', x, y, value);
    }
    else {
        throw std::runtime_error("Invalid channel color especification in call to function AssignValue(" + component + ')');
    }

}

Image Image::ReadBMP(const std::string& filePath) {

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
        image.GreenComponent.push_back((img[(size_t)i + 1] & 0xff));
        image.RedComponent.push_back((img[(size_t)i + 2] & 0xff));

    }

    return image;
}

void Image::WriteBMP(const std::string& filePath) const {

    std::ofstream bmp(filePath, std::ios::binary);

    if (bmp.fail()) {
        throw std::runtime_error("File " + filePath + " cannot be opened to write.");
    }

    // File Header
    this->fileHeader.Write(bmp);

    // Info Header
    this->infoHeader.Write(bmp);

    // Pixel Write 
    for (int h = this->Height - 1; h >= 0; h--) {

        int beginFactor = h * this->Width;
        int endFactor = beginFactor + this->Width;

        std::vector<unsigned char> r(this->RedComponent.begin() + beginFactor, this->RedComponent.begin() + endFactor);
        std::vector<unsigned char> g(this->GreenComponent.begin() + beginFactor, this->GreenComponent.begin() + endFactor);
        std::vector<unsigned char> b(this->BlueComponent.begin() + beginFactor, this->BlueComponent.begin() + endFactor);

        for (int i = r.size() - 1; i >= 0; i--) {

            bmp.write((char*)&b[i], 1);
            bmp.write((char*)&g[i], 1);
            bmp.write((char*)&r[i], 1);

        }

    }

    bmp.close();
}

Image Image::AdaptToGrayScale(const Image& Img) {

    Image GreyScale = Image(Img);

    for (int row = 0; row < Img.Height; row++) {

        for (int column = 0; column < Img.Width; column++) {

            unsigned char r = Img.RetrieveValue('R', row, column);
            unsigned char g = Img.RetrieveValue('G', row, column);
            unsigned char b = Img.RetrieveValue('B', row, column);

            unsigned char value = (unsigned char)(((double)r*0.21 + (double)g*0.72 + (double)b*0.07)); // HDTV Conversion

            GreyScale.AssignValue('R', row, column, value);
            GreyScale.AssignValue('G', row, column, value);
            GreyScale.AssignValue('B', row, column, value);
        }
    }

    return GreyScale;
}


unsigned char* Image::ToArray() const{

    unsigned char* Img = new unsigned char[(size_t)(this->Width) * (size_t)(this->Height)];

    for (int i = 0; i < this->Height; i++) {

        for (int j = 0; j < this->Width; j++) {

            int x1 = i * Width;
            int y1 = Height - j - 1;
            Img[x1+y1] = this->RetrieveValue('R', i, j);

        }

    }

    return Img;

}

unsigned char** Image::MyToArray() const {

    unsigned char** Img = new unsigned char* [(size_t)(this->Height + 2)];
    for (size_t i = 0; i < this->Height + 2; i++) {
        Img[i] = new unsigned char[(size_t)(this->Width + 2)];
    }
    for (int i = 1; i <= this->Height; i++) {
        for (int j = 1; j <= this->Width; j++) {

            Img[i][j] = this->RetrieveValue('R', i - 1, j - 1);
        }
    }
    for (int i = 0; i < this->Width + 2; i++) {
        Img[0][i] = Img[1][i];
        Img[this->Height + 1][i] = Img[this->Height][i];
    }
    for (int i = 0; i < this->Height + 2; i++) {
        Img[i][0] = Img[i][1];
        Img[i][this->Width + 1] = Img[i][this->Width];
    }
    //-------------
    return Img;

}

void Image::FromArray(unsigned char* Img, int Width, int Height){

    for (int fila = 0; fila < Height; fila++) {

        for (int columna = 0; columna < Width; columna++) {

            unsigned char value = Img[Image::AdaptCoords(fila, columna, Width, Height)];
            this->RedComponent[Image::AdaptCoords(fila, columna, Width, Height)] = value;
            this->GreenComponent[Image::AdaptCoords(fila, columna, Width, Height)] = value;
            this->BlueComponent[Image::AdaptCoords(fila, columna, Width, Height)] = value;

        }
    }

}

int Image::AdaptCoords(int x, int y)
{
    return Image::AdaptCoords(x, y, this->Width, this->Height);
}

int Image::AdaptCoords(int x, int y, int Width, int Height)
{
    if (x < 0 || x >= Height || y < 0 || y >= Width) {
        return -1;
    }

    int x1 = x * Width;
    int y1 = Height - y - 1;
    return x1 + y1;
}
