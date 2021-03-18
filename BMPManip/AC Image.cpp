#include "Headers/BMPManip.h"
#include <ctime>
#include <algorithm>
#include <Windows.h>

using namespace std;

Image CPPOperation(Image& Img);
Image ASMOperation(Image& Img);
Image SSEOperation(Image& Img);
void Results(double CppTime, double AsmTime, double SseTime);
unsigned char Median(const Image& Img, char component, int x, int y);

int main(int argc, char** argv) {

    if (argc < 2) {

        cerr << "Incorrect number of parameters. Expected: 3, Obtained: " << argc << endl;
        return -1;

    }

    try {

        Image Imagen = Image::ReadBMP(string(argv[1]));
        Image GreyScale = Image::AdaptToGrayScale(Imagen);

        auto begin = clock();
        Image Cpp = CPPOperation(GreyScale);
        auto end = clock();
        double CppTime = double(end) - double(begin) / CLOCKS_PER_SEC;

        begin = clock();
        Image Asm = ASMOperation(GreyScale);
        end = clock();
        double AsmTime = double(end) - double(begin) / CLOCKS_PER_SEC;

        begin = clock();
        Image Sse = SSEOperation(GreyScale);
        end = clock();
        double SseTime = double(end) - double(begin) / CLOCKS_PER_SEC;

        Cpp.WriteBMP("Output\\output.bmp");
        ShellExecute(0, 0, L"Output\\output.bmp", 0, 0, SW_SHOW);

        Results(CppTime, AsmTime, SseTime);

    }
    catch (runtime_error Ex) {

        cerr << "An exception has been ocurred. Please review the following error description:\n \t" << Ex.what() << endl;
        return -1;

    }
 
    return 0;
}

Image CPPOperation(Image& Img){

    Image Cpp = Image(Img);

    for (int fila = 0; fila < Cpp.Height; fila++) {

        for (int columna = 0; columna < Cpp.Width; columna++) {

            unsigned char value = Median(Img, 'R', fila, columna);
            Cpp.AssignValue('A', fila, columna, value);

        }
    }

    return Cpp;
}

Image ASMOperation(Image& Img)
{
    return Img;
}

Image SSEOperation(Image& Img)
{
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

unsigned char Median(const Image& Img, char component, int x, int y) {
    
    vector<unsigned char> AdjacentValues;

    for (int x1 = x - 1; x1 <= x + 1; x1++) {

        for (int y1 = y - 1; y1 <= y + 1; y1++) {

            char value = Img.RetrieveValue(component, x1, y1);

            if (value != -1) {
                AdjacentValues.push_back(value);
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