#include "BMPManip.h"

using namespace std;

namespace Testing {

    int main(int argc, char** argv) {

        if (argc < 3) {

            cerr << "Incorrect number of parameters. Expected: 3, Obtained: " << argc << endl;
            return -1;

        }

        try {

            Image Imagen1 = Image::ReadBMP(string(argv[3]));
            Image Imagen2 = Image::ReadBMP(string(argv[2]));

            Imagen1.AssignValue('A', 0, 0, 0);
            Imagen1.AssignValue('A', Imagen1.Width - 1, 0, 0);
            Imagen1.AssignValue('A', 0, Imagen1.Width - 1, 0);
            Imagen1.AssignValue('A', Imagen1.Width - 1, Imagen1.Width - 1, 0);

            unsigned char p31_0 = Imagen1.RetrieveValue('R', Imagen1.Width - 1, 0);
            unsigned char p31_1 = Imagen1.RetrieveValue('R', Imagen1.Width - 1, 1);
            unsigned char p30_0 = Imagen1.RetrieveValue('R', Imagen1.Width - 2, 0);
            unsigned char p30_1 = Imagen1.RetrieveValue('R', Imagen1.Width - 2, 1);

            Imagen1.WriteBMP("Output\\output.bmp");

        }
        catch (string Ex) {

            cerr << "An exception has been ocurred. Please review the following error description:\n \t" << Ex << endl;
            return -1;

        }

        return 0;
    }

}