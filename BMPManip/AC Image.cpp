#include "Headers/BMPManip.h"
#include <ctime>
#include <algorithm>
#include <Windows.h>

constexpr wchar_t OutputPath[] = L"Output\\output.bmp";

using namespace std;

int main(int argc, char** argv);
unsigned char* CPPOperation(unsigned char*, int, int);
unsigned char* ASMOperation(unsigned char*, int, int);
unsigned char* SSEOperation(unsigned char*, int, int);
void Results(double, double, double);
unsigned char Median(unsigned char*, char, int, int, int, int);
int AdaptCoords(int, int, int, int);
void BubbleSort(unsigned char*, int);

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

    return Img;
    
}

unsigned char* ASMOperation(unsigned char* Img, int Width, int Height) {
 
    return Img;

}

unsigned char* SSEOperation(unsigned char* Img, int Width, int Height) {

    return Img;

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
    
    unsigned char* AdjacentValues = new unsigned char[9];
    int AdjacentValuesSize = 0;

    for (int x1 = x - 1; x1 <= x + 1; x1++) {

        for (int y1 = y - 1; y1 <= y + 1; y1++) {

            char pos = AdaptCoords(x1, y1, Width, Height);

            if (pos != -1) {
                AdjacentValues[AdjacentValuesSize] = Img[pos];
                AdjacentValuesSize++;
            }
        }
    }

    /*
    
    EQUIVALENCIA PARA ASM
    Podeis hacer este codigo 9 veces en vez de un doble bucle, asegurandoos cambiar el x1 -1 y el y1 -1 para las diferentes posiciones.

    char pos;
    pos = AdaptCoords(x1-1, y1-1, Width, Height);

    if (pos != -1) {
        AdjacentValues[AdjacentValuesSize] = Img[pos];
        AdjacentValuesSize++;
    }
  
    
    */

    BubbleSort(AdjacentValues, AdjacentValuesSize);

    if (AdjacentValuesSize % 2 == 0) {

        return AdjacentValues[(AdjacentValuesSize / 2) - 1] + AdjacentValues[(AdjacentValuesSize / 2)] / 2;

    }
    else {

        return AdjacentValues[(AdjacentValuesSize/ 2)];
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

void BubbleSort(unsigned char* arr, int n) {

    for (int i = 0; i < n - 1; i++) {

        for (int j = 0; j < n - i - 1; j++) {

            if (arr[j] > arr[j + 1]) {

                unsigned char temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

            }
        }
    }
}
