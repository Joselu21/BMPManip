#include "BMPManip.h"
#include <ctime>
#include <algorithm>
#include <Windows.h>

using namespace std;

Image CPPOperation(Image& Img);
Image ASMOperation(Image& Img);
Image SSEOperation(Image& Img);
void Results(double CppTime, double AsmTime, double SseTime);
char Median(const Image& Img, char component, int x, int y);

int main(int argc, char** argv) {

    if (argc < 2) {

        cerr << "Incorrect number of parameters. Expected: 3, Obtained: " << argc << endl;
        return -1;

    }

    try {

        Image Imagen = Image::ReadBMP(string(argv[1]));

        auto begin = clock();
        Image Cpp = CPPOperation(Imagen);
        auto end = clock();
        double CppTime = double(end) - double(begin) / CLOCKS_PER_SEC;

        begin = clock();
        Image Asm = ASMOperation(Imagen);
        end = clock();
        double AsmTime = double(end) - double(begin) / CLOCKS_PER_SEC;

        begin = clock();
        Image Sse = SSEOperation(Imagen);
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

    for (int fila = 0; fila < Img.Height; fila++) {

        for (int columna = 0; columna < Img.Width; columna++) {

            Img.AssignValue('R', fila, columna, Median(Img, 'R', fila, columna));
            Img.AssignValue('G', fila, columna, Median(Img, 'G', fila, columna));
            Img.AssignValue('B', fila, columna, Median(Img, 'B', fila, columna));

        }

    }

    return Img;

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
        << "Assembly Time: " << AsmTime << endl << endl
        << (CppTime < AsmTime ? (CppTime < SseTime ? "C++" : "SSE") : (AsmTime < SseTime ? "ASM" : "SSE")) << " time is faster" << endl;

}

char Median(const Image& Img, char component, int x, int y) {
    
    vector<int> array_8;
    int order = 0, cambio = 0,median_pair=0;

    if (Img.RetrieveValue(component, x - 1, y - 1) != -1) {
        array_8.push_back(Img.RetrieveValue(component, x - 1, y - 1));
    }

    if (Img.RetrieveValue(component, x, y - 1) != -1) {
        array_8.push_back(Img.RetrieveValue(component, x, y - 1));
    }

    if (Img.RetrieveValue(component, x + 1, y - 1) != -1) {
        array_8.push_back(Img.RetrieveValue(component, x + 1, y - 1));
    }

    if (Img.RetrieveValue(component, x - 1, y) != -1) {
        array_8.push_back(Img.RetrieveValue(component, x - 1, y));
    }

    if (Img.RetrieveValue(component, x + 1, y) != -1) {
        array_8.push_back(Img.RetrieveValue(component, x + 1, y));
    }

    if (Img.RetrieveValue(component, x - 1, y + 1) != -1) {
        array_8.push_back(Img.RetrieveValue(component, x - 1, y + 1));
    }

    if (Img.RetrieveValue(component, x, y + 1) != -1){
        array_8.push_back(Img.RetrieveValue(component, x, y + 1));
    }

    if (Img.RetrieveValue(component, x + 1, y + 1) != -1){
        array_8.push_back(Img.RetrieveValue(component, x + 1, y + 1));
    }


    sort(array_8.begin(), array_8.end());

    if (array_8.size() % 2 == 0) {

        return array_8[array_8.size() / 2] + array_8[(array_8.size() / 2) + 1] / 2;
    }
    else {

        return array_8[(array_8.size()/ 2) + 1];

    }

}





     

