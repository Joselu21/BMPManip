#include "BMPManip.h"
#include <ctime>

using namespace std;

Image CPPOperation(const Image& Img1, const Image& Img2);
Image ASMOperation(const Image& Img1, const Image& Img2);

int main(int argc, char** argv) {

    if (argc != 3) {

        cerr << "Incorrect number of parameters. Expected: 3, Obtained: " << argc << endl;
        return -1;

    }

    try {

        Image Imagen1 = Image::ReadBMP(string(argv[1]));
        Image Imagen2 = Image::ReadBMP(string(argv[2]));

        auto begin = clock();
        Image Cpp = CPPOperation(Imagen1, Imagen2);
        auto end = clock();
        double CppTime = double(end - begin) / CLOCKS_PER_SEC;

        auto begin = clock();
        Image Cpp = ASMOperation(Imagen1, Imagen2);
        auto end = clock();
        double AsmTime = double(end - begin) / CLOCKS_PER_SEC;

        Results(CppTime, AsmTime);

    }
    catch (string Ex) {

        cerr << "An exception has been ocurred. Please review the following error description:\n \t" << Ex << endl;
        return -1;

    }
 
    return 0;
}

Image CPPOperation(const Image& Img1, const Image& Img2)
{
    return Image(1,1);
}

Image ASMOperation(const Image& Img1, const Image& Img2)
{
    return Image(1,1);
}

void Results(double CppTime, double AsmTime) {

    cout << "The program has terminated correctly." << endl
        << "These are the results: " << endl
        << "C++ Time: " << CppTime << endl
        << "Assembly Time: " << AsmTime << endl << endl
        << (CppTime < AsmTime ? "C++" : "Assembly") << " time is faster by " << abs(CppTime - AsmTime) / 1000.0 << " seconds." << endl;

}
