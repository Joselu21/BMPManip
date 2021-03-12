#include "BMPManip.h"
#include <ctime>

using namespace std;

Image CPPOperation(const Image& Img1, const Image& Img2);
Image ASMOperation(const Image& Img1, const Image& Img2);
Image SSEOperation(const Image& Img1, const Image& Img2);
void Results(double CppTime, double AsmTime, double SseTime);

int main(int argc, char** argv) {

    if (argc < 3) {

        cerr << "Incorrect number of parameters. Expected: 3, Obtained: " << argc << endl;
        return -1;

    }

    try {

        Image Imagen1 = Image::ReadBMP(string(argv[3]));
        Image Imagen2 = Image::ReadBMP(string(argv[2]));

        auto begin = clock();
        Image Cpp = CPPOperation(Imagen1, Imagen2);
        auto end = clock();
        double CppTime = double(end - begin) / CLOCKS_PER_SEC;

        begin = clock();
        Image Asm = ASMOperation(Imagen1, Imagen2);
        end = clock();
        double AsmTime = double(end - begin) / CLOCKS_PER_SEC;

        begin = clock();
        Image Sse = SSEOperation(Imagen1, Imagen2);
        end = clock();
        double SseTime = double(end - begin) / CLOCKS_PER_SEC;

        Cpp.WriteBMP("Output\\output.bmp");

        Results(CppTime, AsmTime, SseTime);

    }
    catch (string Ex) {

        cerr << "An exception has been ocurred. Please review the following error description:\n \t" << Ex << endl;
        return -1;

    }
 
    return 0;
}

Image CPPOperation(const Image& Img1, const Image& Img2)
{
    return Img1;
}

Image ASMOperation(const Image& Img1, const Image& Img2)
{
    return Img1;
}

Image SSEOperation(const Image& Img1, const Image& Img2)
{
    return Img1;
}

void Results(double CppTime, double AsmTime, double SseTime) { // TODO

    cout << "The program has terminated correctly." << endl
        << "These are the results: " << endl
        << "C++ Time: " << CppTime << endl
        << "Assembly Time: " << AsmTime << endl << endl
        << (CppTime < AsmTime ? "C++" : "Assembly") << " time is faster by " << abs(CppTime - AsmTime) / 1000.0 << " seconds." << endl;

}
