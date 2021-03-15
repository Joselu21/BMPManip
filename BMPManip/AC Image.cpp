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
    int array_3[3];
    int array_8[8];
    int order = 0, cambio = 0, mediana_5 = 0;

    if (x == 0 && y == 0) {
        //esquina superior izquierda
        array_3[0] = Img.RetrieveValue(component, 1, 0);
        array_3[1] = Img.RetrieveValue(component, 1, 1);
        array_3[2] = Img.RetrieveValue(component, 0, 1);

        while (order < 2) {
            //
            for (int i = 0;i < 2;i++) {
                if (array_3[i] < array_3[i + 1]) {
                    order++;
                }
                else {
                    cambio = array_3[i];
                    array_3[i] = array_3[i + 1];
                    array_3[i + 1] = cambio;
                    order = 0;
                }
            }

        }
        return array_3[1];
    }
    else if (x == 0 && (y + 1) == Img.Height) {
        // esquina inferior izquierda
        array_3[0] = Img.RetrieveValue(component, 0, (y - 1));
        array_3[1] = Img.RetrieveValue(component, 1, (y - 1));
        array_3[2] = Img.RetrieveValue(component, 0, y);

        while (order < 2) {

            for (int i = 0;i < 2;i++) {
                if (array_3[i] < array_3[i + 1]) {
                    order++;
                }
                else {
                    cambio = array_3[i];
                    array_3[i] = array_3[i + 1];
                    array_3[i + 1] = cambio;
                    order = 0;
                }
            }

        }
        return array_3[1];
    }
    else if (y == 0 && (x + 1) == Img.Width) {
        //esquina superior derecha
        array_3[0] = Img.RetrieveValue(component, x - 1, 0);
        array_3[1] = Img.RetrieveValue(component, x - 1, 1);
        array_3[2] = Img.RetrieveValue(component, x, 1);

        while (order < 2) {

            for (int i = 0;i < 2;i++) {
                if (array_3[i] < array_3[i + 1]) {
                    order++;
                }
                else {
                    cambio = array_3[i];
                    array_3[i] = array_3[i + 1];
                    array_3[i + 1] = cambio;
                    order = 0;
                }
            }

        }
        return array_3[1];
    }
    else if ((x + 1) == Img.Width && (y + 1) == Img.Height) {
        //esquina inferior derecha
        array_3[0] = Img.RetrieveValue(component, x - 1, y);
        array_3[1] = Img.RetrieveValue(component, x - 1, y - 1);
        array_3[2] = Img.RetrieveValue(component, x, y - 1);
        int order = 0, cambio = 0;
        while (order < 2) {

            for (int i = 0;i < 2;i++) {
                if (array_3[i] < array_3[i + 1]) {
                    order++;
                }
                else {
                    cambio = array_3[i];
                    array_3[i] = array_3[i + 1];
                    array_3[i + 1] = cambio;
                    order = 0;
                }
            }

        }
        return array_3[1];
    }
    else {
        // caso normal

        array_8[0] = Img.RetrieveValue(component, x - 1, y - 1);
        array_8[1] = Img.RetrieveValue(component, x, y - 1);
        array_8[2] = Img.RetrieveValue(component, x + 1, y - 1);
        array_8[3] = Img.RetrieveValue(component, x - 1, y);
        array_8[4] = Img.RetrieveValue(component, x + 1, y);
        array_8[5] = Img.RetrieveValue(component, x - 1, y + 1);
        array_8[6] = Img.RetrieveValue(component, x, y + 1);
        array_8[7] = Img.RetrieveValue(component, x + 1, y + 1);


        while (order < 7) {

            for (int i = 0;i < 7;i++) {
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

        mediana_5 = (array_8[3] + array_8[4]) / 2;
        return mediana_5;
    }
}





     
}
