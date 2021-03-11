#include "BMPManip.h"

using namespace std;

int main(int argc, char** argv) {

    if (argc != 3) {

        cerr << "Incorrect number of parameters. Expected: 3, Obtained: " << argc << endl;
        return -1;

    }
 
    Image Imagen1 = Image::ReadBMP(string(argv[1]));
    Imagen1.WriteBMP("Output\\out-" + string(argv[1]));

    return 0;
}

