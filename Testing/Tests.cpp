#include "../BMPManip/BMPManip.h"

using namespace std;

int main(int argc, char** argv) {

    if (argc < 3) {
        cerr << "Invalid number of arguments. \nExpected at least 3, but obtained only " << argc << endl;
    }

    try {

        Image Imagen= Image::ReadBMP(filePath);

        Imagen.AssignValue('A', 0, 0, 0);
        Imagen.AssignValue('A', Imagen.Width - 1, 0, 0);
        Imagen.AssignValue('A', 0, Imagen.Width - 1, 0);
        Imagen.AssignValue('A', Imagen.Width - 1, Imagen.Width - 1, 0);

        unsigned char p31_0 = Imagen.RetrieveValue('R', Imagen.Width - 1, 0);
        unsigned char p31_1 = Imagen.RetrieveValue('R', Imagen.Width - 1, 1);
        unsigned char p30_0 = Imagen.RetrieveValue('R', Imagen.Width - 2, 0);
        unsigned char p30_1 = Imagen.RetrieveValue('R', Imagen.Width - 2, 1);

        if (!output.empty()) {
            Imagen.WriteBMP("Output\\output.bmp");
        }

    }
    catch (runtime_error Ex) {

        cerr << "An exception has been ocurred. Please review the following error description:\n \t" << Ex.what() << endl;
        return -1;

    }

    return 0;
}

