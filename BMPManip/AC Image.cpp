#include "Headers/BMPManip.h"
#include <ctime>
#include <algorithm>
#include <Windows.h>
#include <chrono>

constexpr auto OutputPath = "Output\\output.bmp";

using namespace std;

int main(int argc, char** argv);
unsigned char* CPPOperation(unsigned char*, int, int);
unsigned char* ASMOperation(unsigned char*, int, int);
unsigned char* SSEOperation(unsigned char*, int, int);
void Results(double, double, double);
unsigned char Median(unsigned char*, char, int, int, int, int);
int AdaptCoords(int, int, int, int);
void BubbleSort(unsigned char*, int);
void BubbleSortSSE(unsigned char*, int);
unsigned char ASMMedian(unsigned char*, char, int, int, int, int);
unsigned char SSEMedian(unsigned char*, char, int, int, int, int);

int main(int argc, char** argv) {

    if (argc < 2) {

        cerr << "Incorrect number of parameters. Expected: 3, Obtained: " << argc << endl;
        return -1;

    }

    try {

        Image Imagen = Image::ReadBMP(string(argv[1]));
        unsigned char* GreyScale = Image::AdaptToGrayScale(Imagen).Order();

        auto begin = chrono::high_resolution_clock::now();
        unsigned char* Cpp = CPPOperation(GreyScale, Imagen.Width, Imagen.Height);
        auto end = chrono::high_resolution_clock::now();
        double CppTime = chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        begin = chrono::high_resolution_clock::now();
        unsigned char* Asm = ASMOperation(GreyScale, Imagen.Width, Imagen.Height);
        end = chrono::high_resolution_clock::now();
        double AsmTime = chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        begin = chrono::high_resolution_clock::now();
        unsigned char* Sse = SSEOperation(GreyScale, Imagen.Width, Imagen.Height);
        end = chrono::high_resolution_clock::now();
        double SseTime = chrono::duration_cast<std::chrono::microseconds>(end - begin).count();


        Imagen.WriteBMP((char*)(OutputPath));
        ShellExecute(0, 0, (const wchar_t*)OutputPath, 0, 0, SW_SHOW); // TODO
        
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
 
    for (int fila = 0; fila < Height; fila++) {

        for (int columna = 0; columna < Width; columna++) {

            unsigned char value = ASMMedian(Img, 'R', fila, columna, Width, Height);
            Img[AdaptCoords(fila, columna, Width, Height)] = value;

        }
    }

    return Img;

}

unsigned char* SSEOperation(unsigned char* Img, int Width, int Height) {

    for (int fila = 0; fila < Height; fila++) {

        for (int columna = 0; columna < Width; columna++) {

            unsigned char value = SSEMedian(Img, 'R', fila, columna, Width, Height);
            Img[AdaptCoords(fila, columna, Width, Height)] = value;

        }
    }

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

            unsigned char pos = AdaptCoords(x1, y1, Width, Height);

            if (pos != -1) {
                AdjacentValues[AdjacentValuesSize] = Img[pos];
                AdjacentValuesSize++;
            }
        }
    }

    BubbleSort(AdjacentValues, AdjacentValuesSize);

    if (AdjacentValuesSize % 2 == 0) {

        return AdjacentValues[(AdjacentValuesSize / 2) - 1] + AdjacentValues[(AdjacentValuesSize / 2)] / 2;

    }
    else {

        return AdjacentValues[(AdjacentValuesSize/ 2)];
    }

}

unsigned char ASMMedian(unsigned char* ImgChar, char component, int x, int y, int Width, int Height) {
    const int tam = Width * Height;
    int AdjacentValuesSize = 0;

    unsigned int* Img = new unsigned int[tam];
    unsigned int* AdjacentValues = new unsigned int[9];

    //***CALCULO DE LA MEDIANA DE UNA COMPONENTE MATRICIAL***

    //Casting solo posible en C++
    for (int i = 0;i < tam;i++)
    {
        Img[i] = ImgChar[i];
    }

    //Variables para el entorno de ASM

    int tamElem = sizeof(AdjacentValues[0]);
    int tamVector = tamElem * 9;

    unsigned int elem = 0;
    unsigned int elemSig = 0;
    unsigned int dirMemoria = 0;
    unsigned int dirMemoriaSig = 0;
    unsigned int mediana = 0;
    unsigned int mediana2 = 0;
    unsigned int mediana3 = 0;
    unsigned int medianaTotal = 0;

    int i = 0;
    int j = 0;
    int xIni = 0;
    int yIni = 0;
    int xFin = 0;
    int yFin = 0;
    int pos = 0;
    int posImg = 0;

    _asm {

        ValuesAdjacent:
        mov eax, x
            mov xIni, eax
            sub xIni, 1
            mov xFin, eax
            add xFin, 1
            mov edx, xFin

            bucleExt1 :
            cmp xIni, edx
            jg finBucleExt1
            mov eax, y
            mov yIni, eax
            sub yIni, 1
            mov yFin, eax
            add yFin, 1
            mov ecx, yFin

            bucleInt1 :
            cmp yIni, ecx
            jg finBucleInt1

            AdaptCoords :
            mov eax, xIni
            mov ebx, yIni
            cmp eax, 0
            jl superaLimites
            cmp ebx, 0
            jl superaLimites
            cmp eax, Height
            jge superaLimites
            cmp ebx, Width
            jge superaLimites

            mov eax, Width
            mul xIni
            mov ecx, Height
            sub ecx, ebx
            sub ecx, 1
            add eax, ecx
            jmp finAdaptCoords

            superaLimites :
            mov eax, -1
            jmp finAdaptCoords

            finAdaptCoords :
            mov pos, eax

            cmp pos, -1
            je incYIni
            mov ecx, Img
            mov eax, pos
            mov ebx, tamElem
            mul ebx
            add ecx, eax
            mov edx, [ecx]
            mov posImg, edx

            mov eax, AdjacentValuesSize
            mov ebx, tamElem
            mul ebx
            mov ecx, AdjacentValues
            add ecx, eax
            mov edx, posImg
            mov[ecx], edx
            add AdjacentValuesSize, 1
            jmp incYIni

            incYIni :
            add yIni, 1
            mov ecx, yFin
            jmp bucleInt1

            finBucleInt1 :
            add xIni, 1
            mov edx, xFin
            jmp bucleExt1

            finBucleExt1 :
            jmp finValuesAdjacent

            finValuesAdjacent :
            jmp bubbleSort

            bubbleSort :
            mov i, 2
            mov j, 0
            mov edx, 2
            mov ecx, 0
            sub AdjacentValuesSize, 1

            bucleExt2 :
            cmp edx, AdjacentValuesSize
            je finBucleExt2
            mov eax, AdjacentValues
            mov ecx, 0
            bucleInt2 :

            cmp ecx, AdjacentValuesSize
            je finBucleInt2

            mov ebx, [eax]
            mov elem, ebx
            mov dirMemoria, eax
            add eax, tamElem
            mov dirMemoriaSig, eax
            mov ebx, [eax]
            mov elemSig, ebx
            mov ebx, elem
            cmp ebx, elemSig
            jg Swap

            noSwap :
            inc ecx
            jmp bucleInt2

            Swap :
            inc ecx
            mov j, ecx
            mov i, edx
            mov ebx, dirMemoria
            mov ecx, elem
            mov edx, elemSig
            mov[eax], ecx
            mov[ebx], edx
            mov ecx, j
            mov edx, i
            mov eax, dirMemoriaSig
            jmp bucleInt2

            finBucleInt2 :
            inc edx
            jmp bucleExt2

            finBucleExt2 :
            jmp finBubbleSort

            finBubbleSort :
            jmp Mediana

            Mediana :
            add AdjacentValuesSize, 1
            mov ecx, AdjacentValues
            mov eax, AdjacentValuesSize
            mov edx, 0
            mov ebx, 2

            div ebx
            mov ebx, 0
            cmp edx, ebx
            je esPar
            jne esImpar

            esImpar :
            mov ebx, tamElem
            mul ebx
            add ecx, eax
            mov edx, [ecx]
            mov mediana, edx
            jmp finMediana

            esPar :
            mov ebx, tamElem
            mul ebx
            add ecx, eax
            mov edx, [ecx]
            mov mediana, edx

            sub ecx, tamElem
            mov edx, [ecx]
            mov mediana2, edx

            mov ebx, mediana
            mov ecx, mediana2
            add ecx, ebx
            mov mediana3, ecx

            mov edx, 0
            mov ebx, 2
            mov eax, mediana3

            div ebx
            mov mediana, eax
            jmp finMediana

            finMediana :
            mov eax, mediana
            mov medianaTotal, eax
    }

    return (unsigned char)medianaTotal;
}

unsigned char SSEMedian(unsigned char* ImgChar, char component, int x, int y, int Width, int Height) {

    unsigned char* AdjacentValues = new unsigned char[9];
    int AdjacentValuesSize = 0;

    for (int x1 = x - 1; x1 <= x + 1; x1++) {

        for (int y1 = y - 1; y1 <= y + 1; y1++) {

            unsigned char pos = AdaptCoords(x1, y1, Width, Height);

            if (pos != -1) {
                AdjacentValues[AdjacentValuesSize] = ImgChar[pos];
                AdjacentValuesSize++;
            }
        }
    }

    BubbleSortSSE(AdjacentValues, AdjacentValuesSize);

    if (AdjacentValuesSize % 2 == 0) {

        return AdjacentValues[(AdjacentValuesSize / 2) - 1] + AdjacentValues[(AdjacentValuesSize / 2)] / 2;

    }
    else {

        return AdjacentValues[(AdjacentValuesSize / 2)];
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

void BubbleSortSSE(unsigned char* arr, int n) {



}