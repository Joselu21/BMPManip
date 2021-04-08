#include "Headers/BMPManip.h"
#include <ctime>
#include <algorithm>
#include <Windows.h>

constexpr wchar_t OutputPath[] = L"Output\\output.bmp";

using namespace std;

int main(int argc, char** argv);
unsigned char* CPPOperation(const unsigned char*, int, int);
unsigned char* ASMOperation(const unsigned char*, int, int);
unsigned char* SSEOperation(const unsigned char*, int, int);
void Results(double CppTime, double AsmTime, double SseTime);
unsigned char Median(unsigned char* Img, char component, int x, int y, int Width, int Height);
int AdaptCoords(int x, int y, int Width, int Height);

int main(int argc, char** argv) {

    if (argc < 2) {

        cerr << "Incorrect number of parameters. Expected: 3, Obtained: " << argc << endl;
        return -1;

    }

    try {

        Image Imagen = Image::ReadBMP(string(argv[1]));
        unsigned char* GreyScale = Image::AdaptToGrayScale(Imagen).Order();

        auto begin = clock();
        unsigned char* Cpp = CPPOperation(GreyScale, Imagen.Width, Imagen.Height);
        auto end = clock();
        double CppTime = double(end) - double(begin) / CLOCKS_PER_SEC;

        begin = clock();
        unsigned char* Asm = ASMOperation(GreyScale, Imagen.Width, Imagen.Height);
        end = clock();
        double AsmTime = double(end) - double(begin) / CLOCKS_PER_SEC;

        begin = clock();
        unsigned char* Sse = SSEOperation(GreyScale, Imagen.Width, Imagen.Height);
        end = clock();
        double SseTime = double(end) - double(begin) / CLOCKS_PER_SEC;


        Imagen.WriteBMP((char*)(OutputPath));
        ShellExecute(0, 0, OutputPath, 0, 0, SW_SHOW);
        
        Results(CppTime, AsmTime, SseTime);

    }
    catch (runtime_error Ex) {

        cerr << "An exception has been ocurred. Please review the following error description:\n \t" << Ex.what() << endl;
        return -1;

    }
 
    return 0;
}

unsigned char* CPPOperation(unsigned char* Img, int Width, int Height){

    for (int fila = 0; fila < Height; fila++) {

        for (int columna = 0; columna < Width; columna++) {

            unsigned char value = Median(Img, 'R', fila, columna, Width, Height);
            Img[AdaptCoords(fila, columna, Width, Height)] = value;

        }
    }
    
}

unsigned char* ASMOperation(unsigned char* Img, int Width, int Height) {
 


}

unsigned char* SSEOperation(unsigned char* Img, int Width, int Height) {



}

void Results(double CppTime, double AsmTime, double SseTime) {

    cout << "The program has terminated correctly." << endl
        << "These are the results: " << endl
        << "C++ Time: " << CppTime << endl
        << "Assembly Time: " << AsmTime << endl
        << "SSE Time: " << SseTime << endl << endl
        << (CppTime < AsmTime ? (CppTime < SseTime ? "C++" : "SSE") : (AsmTime < SseTime ? "ASM" : "SSE")) << " time is faster" << endl;

}

unsigned char Median(unsigned char* Img, char component, int x, int y, int Width, int Height) {
    
    vector<unsigned char> AdjacentValues;

    for (int x1 = x - 1; x1 <= x + 1; x1++) {

        for (int y1 = y - 1; y1 <= y + 1; y1++) {

            char pos = AdaptCoords(x1, y1, Width, Height);

            if (pos != -1) {
                AdjacentValues.push_back(Img[pos]);
            }
        }
    }

    sort(AdjacentValues.begin(), AdjacentValues.end());

    if (AdjacentValues.size() % 2 == 0) {

        return AdjacentValues[(AdjacentValues.size() / 2) - 1] + AdjacentValues[(AdjacentValues.size() / 2)] / 2;

    }
    else {

        return AdjacentValues[(AdjacentValues.size()/ 2)];

    }

}

int AdaptCoords(int x, int y, int Width, int Height)
{
    if (x < 0 || x >= Height || y < 0 || y >= Width) {
        return -1;
    }

    int x1 = x * Width;
    int y1 = Height - y - 1;
    return x1 + y1;
}
