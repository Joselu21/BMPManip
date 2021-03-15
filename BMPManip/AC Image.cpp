#include "BMPManip.h"
#include <ctime>
#include <Windows.h>

using namespace std;

Image CPPOperation(const Image& Img);
Image ASMOperation(const Image& Img);
Image SSEOperation(const Image& Img);
void Results(double CppTime, double AsmTime, double SseTime);
char Median(const Image& Img, char component, int x, int y);

int main(int argc, char** argv) {

    if (argc < 3) {

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

Image CPPOperation(const Image& Img)
{
    return Img;
}

Image ASMOperation(const Image& Img)
{
    return Img;
}

Image SSEOperation(const Image& Img)
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


    while (order < (array_8.size()-1)) {

        for (int i = 0;i < (array_8.size() - 1);i++) {
            if (array_8[i] < array_8[i + 1]) {
                order++;
            }
            else {
                cambio = array_8[i];
                array_8[i] = array_8[i + 1];
                array_8[i + 1] = cambio;
                order = 0;
            }
        }

    }

    if (array_8.size() % 2 == 0) {
        median_pair = (array_8[array_8.size() / 2] + array_8[(array_8.size() / 2) + 1])/2;
        
        return median_pair;
    }
    else {
        return array_8[(array_8.size()/2) + 0.5];
    }

    
}





     

