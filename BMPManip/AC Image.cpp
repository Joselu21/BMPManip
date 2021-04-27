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
void SSEOperation(unsigned char**, unsigned char*, int, int);
void Results(double, double, double);
unsigned char Median(unsigned char*, char, int, int, int, int);
int AdaptCoords(int, int, int, int);
void BubbleSort(unsigned char*, int);
void printImg(unsigned char** img, int w, int h);
void printImg(unsigned char* img, int w, int h);

int main(int argc, char** argv) {

    if (argc < 2) {

        cerr << "Incorrect number of parameters. Expected: 3, Obtained: " << argc << endl;
        return -1;

    }

    try {

        Image Imagen = Image::ReadBMP(string(argv[1]));
        unsigned char* GreyScale = Image::AdaptToGrayScale(Imagen).Order();
        unsigned char** MyGreyScaleNormal = Image::AdaptToGrayScale(Imagen).MyOrderNormal();
        unsigned char** MyGreyScale = Image::AdaptToGrayScale(Imagen).MyOrder();

        for (int fila = 0; fila < Imagen.Height; fila++) {

            for (int columna = 0; columna < Imagen.Width; columna++) {

                GreyScale[AdaptCoords(fila, columna, Imagen.Width, Imagen.Height)] = 255;

            }
        }
        printImg(GreyScale, Imagen.Width, Imagen.Height);

        auto begin = chrono::high_resolution_clock::now();
        unsigned char* Cpp = CPPOperation(GreyScale, Imagen.Width, Imagen.Height);
        auto end = chrono::high_resolution_clock::now();
        double CppTime = chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        begin = chrono::high_resolution_clock::now();
        //unsigned char* Asm = ASMOperation(GreyScale, Imagen.Width, Imagen.Height);
        end = chrono::high_resolution_clock::now();
        double AsmTime = chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        
        //cout << "Original" << endl;
        printImg(MyGreyScaleNormal, Imagen.Width, Imagen.Height);
        //cout << "img**: " << MyGreyScale << ", img*: " << MyGreyScale[0] << "img: " << MyGreyScale[0][0] << endl;
        printImg(MyGreyScale, Imagen.Width + 2, Imagen.Height + 2);
        begin = chrono::high_resolution_clock::now();
        SSEOperation(MyGreyScale, GreyScale, Imagen.Width, Imagen.Height);
        end = chrono::high_resolution_clock::now();
        cout << "Resultado SSE: " << endl;
        printImg(MyGreyScale, Imagen.Width + 2, Imagen.Height + 2);
        cout << "Vector final: " << endl;
        printImg(GreyScale, Imagen.Width, Imagen.Height);
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
    cout << endl;

    for (int fila = 0; fila < Height; fila++) {

        for (int columna = 0; columna < Width; columna++) {

            unsigned char value = Median(Img, 'R', fila, columna, Width, Height);
            Img[AdaptCoords(fila, columna, Width, Height)] = value;

        }
    }

    return Img;
}

void SSEOperation(unsigned char** Img, unsigned char* vectorImg, int Width, int Height) {
    // Calculo del filtro de mediana usando CPP
    for (int fila = 0; fila < Height; fila++) {

        for (int columna = 0; columna < Width; columna++) {

            if (fila == 0 || fila == Width-1) {
                
                unsigned char value = Median(vectorImg, 'R', fila, columna, Width, Height);
                vectorImg[AdaptCoords(fila, columna, Width, Height)] = value;
            }
            else if (columna == 0 || columna == Height-1) {
                
                unsigned char value = Median(vectorImg, 'R', fila, columna, Width, Height);
                vectorImg[AdaptCoords(fila, columna, Width, Height)] = value;
            }
            // Son posiciones centrales de la imagen (se calculan con instrucciones SSE)
            else {
                vectorImg[AdaptCoords(fila, columna, Width, Height)] = -1;
            }
        }
    }

    // Valores adyacentes (de 16 pixeles)
    unsigned char** adj = new unsigned char* [9];   // La mediana usa 9 elementos
    for (size_t i = 0; i < 9; i++) {
        adj[i] = new unsigned char[16]; // En un registro xmm caben 16
    }
    for (size_t i = 0; i < 9; i++) {
        for (size_t j = 0; j < 16; j++) {
            adj[i][j] = 1;
        }
    }
    UINT32 ochenta = 0x80808080;
    UINT32* punt = &ochenta;

    _asm {
        mov eax,Height;
        mov ebx, Width;
        mov ecx, 0;       // Contador altura
        mov edx, 0;       // Contador anchura
        mov edi, Img
        add edi, 4        // Saltamos la primera fila

    Bucle1:
        mov esi, [edi];                 // Direccion de la fila
        cmp eax, ecx;
        jle Fin1;

    Bucle2:
        cmp ebx, edx;
        jle Fin2;

        push esi;
        mov esi, [edi - 4];
        movdqu xmm0, [esi + edx];               // Cargamos el superior izda
        movdqu xmm1, [esi + edx + 1];           // Cargamos el superior
        movdqu xmm2, [esi + edx + 2];           // Cargamos el superior dcha
        push edi;

        mov esi, adj;
        mov edi, [esi];
        movdqu [edi], xmm0;                        // adj[0] = superiores izda
        add esi, 4;
        mov edi, [esi];
        movdqu [edi], xmm1;                        // adj[1] = superiores
        add esi, 4;
        mov edi, [esi];
        movdqu [edi], xmm2;                        // adj[2] = superiores dcha
        pop edi;
        pop esi;

        // movdqu carga unaligned, ¿disminuye el rendimiento? ¿preferible movdqa?
        movdqu xmm0, [esi + edx];               // Cargamos los adyacentes a izda
        movdqu xmm1, [esi + edx + 1];           // Cargamos los 16 pixeles
        movdqu xmm2, [esi + edx + 2];           // Cargamos los adyacentes a dcha

        push esi;
        push edi;
        mov esi, adj;
        add esi, 12;
        mov edi, [esi];
        movdqu [edi], xmm0;                        // adj[3] = adyacentes a izda
        add esi, 4;
        mov edi, [esi];
        movdqu [edi], xmm1;                        // adj[4] = pixeles actuales
        add esi, 4;
        mov edi, [esi];
        movdqu [edi], xmm2;                        // adj[5] = adyacentes dcha
        pop edi;
        pop esi;

        push esi;
        mov esi, [edi + 4];
        movdqu xmm0, [esi + edx];               // Cargamos el superior izda
        movdqu xmm1, [esi + edx + 1];           // Cargamos el superior
        movdqu xmm2, [esi + edx + 2];           // Cargamos el superior dcha
        push edi;

        mov esi, adj;
        add esi, 24;
        mov edi, [esi];
        movdqu[edi], xmm0;                        // adj[6] = inferiores izda
        add esi, 4;
        mov edi, [esi];
        movdqu[edi], xmm1;                        // adj[7] = inferiores
        add esi, 4;
        mov edi, [esi];
        movdqu[edi], xmm2;                        // adj[8] = inferiores dcha
        pop edi;
        pop esi;

// Custom BubbleSort
        pushad
        mov eax, 1;                 // eax = i = 1
        mov ecx, 9;                 // tam del vector a ordenar
BucleBubble1:
        cmp ecx, eax;
        jle FinBubble1              // i < tam
        mov ebx, 0;                 // ebx = j = 0

BucleBubble2:
        mov esi, ecx;               // esi = ecx = tam
        sub esi, eax;               // esi = tam - i
        cmp esi, ebx;           
        jle FinBubble2;             // j < tam - i

// Custom swap con SSE
        push eax;
        mov eax, punt;
        movd xmm0, [eax];
        shufps xmm0, xmm0, 0;
        mov esi, adj;
        mov edi, [esi + 4 * ebx];
        movdqu xmm1, [edi];         // v[j]
        add ebx, 1;
        mov edi, [esi + 4 * ebx];
        movdqu xmm2, [edi];         // v[j+1]

        movdqa xmm3, xmm2;          // xmm3 = v[j+1]
        paddb xmm3, xmm0;           // v[j+1] + 0x80
        paddb xmm0, xmm1;           // v[j] + 0x80
        pcmpgtb xmm0, xmm3;         // xmm0 = (v[j] > v[j+1])
        pcmpeqd xmm3, xmm3;         // xmm3 todo a 1
        pxor xmm3, xmm0;            // xmm3 = !(v[j] > v[j+1])
        movdqa xmm4, xmm1;
        pand xmm4, xmm0;            // v[j] AND (v[j]>v[j+1])
        movdqa xmm5, xmm2;
        pand xmm5, xmm3;            // v[j+1] AND !(v[j]>v[j+1])
        por xmm4, xmm5;             // xmm4 v[j+1] despues de los swaps
        pand xmm2, xmm0;            // v[j+1] AND (v[j]>v[j+1])
        pand xmm3, xmm1;            // v[j] AND !(v[j]>v[j+1])
        por xmm3, xmm2;             // xmm3 v[j] despues de los swaps

        movdqu[edi], xmm4;
        sub ebx, 1;
        mov edi, [esi + 4 * ebx];
        movdqu[edi], xmm3;
        pop eax;
        // Fin Custom Swap

        add ebx, 1;
        jmp BucleBubble2

FinBubble2:
        add eax, 1;
        jmp BucleBubble1
FinBubble1: 
        mov eax, adj;
        mov eax, [eax + 16];            // La posicion 4 (4*4=16) contiene las medianas
        movdqu xmm0, [eax];             // Cargamos las 16 medianas
        popad                           // Recuperamos registros anteriores a Bubblesort
// End Custom BubbleSort

        movdqu [esi + edx + 1], xmm0;   // Escribimos 16 medianas

        add edx, 16;
        jmp Bucle2;

    Fin2:
        xor edx, edx;                   // Contador de columnas a 0
        add ecx, 1;                     // Contador filas + 1
        add edi, 4;                     // Siguiente puntero filas
        jmp Bucle1;

    Fin1:

    }

    // Juntar:
    for (int fila = 1; fila < Height - 1; fila++) {
        for (int columna = 1; columna < Width - 1; columna++) {
            vectorImg[AdaptCoords(fila, columna, Width, Height)] = Img[fila+1][columna+1];
        }
    }
}

void Results(double CppTime, double AsmTime, double SseTime) {

    cout << "These are the results: " << endl
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

    /*
    
    EQUIVALENCIA PARA ASM
    Podeis hacer este codigo 9 veces en vez de un doble bucle, asegurandoos cambiar el x1 -1 y el y1 -1 para las diferentes posiciones.

    char pos;
    pos = AdaptCoords(x1-1, y1-1, Width, Height);

    if (pos != -1) {
        AdjacentValues[AdjacentValuesSize] = Img[pos];
        AdjacentValuesSize++;
    }
  
    
    */

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
    unsigned int* AdjacentValues = new unsigned int[tam];

    //***CALCULO DE LA MEDIANA DE UNA COMPONENTE MATRICIAL***

    //Casting solo posible en C++
    for (int i = 0;i < tam;i++)
    {
        Img[i] = ImgChar[i];
    }

    //Variables para el entorno de ASM

    int tamElem = sizeof(AdjacentValues[1]);
    int tamVector = sizeof(AdjacentValues[1]) * tam;

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

    //EJEMPLO MATRIZ
    /*
    *              y
    *          4 3 2 1 0
    *        ------------
    *      0 | a b c d e
    *      1 | f g h i j
    *    x 2 | k l m n o
    *      3 | p q r s t
    *      4 | u v w x y
    *
    * x=1, y=3 -> g
    * x=0, y=0 -> e
    */


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
            jmp BubbleSort

            BubbleSort :
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

    cout << endl;
    cout << "AdjacentValuesSize: " << AdjacentValuesSize << endl;
    cout << "Vector ordenado: ";
    for (int i = 0; i < AdjacentValuesSize;i++)
    {
        if (i != 0) {
            cout << ", ";
        }
        cout << (char)AdjacentValues[i];
    }

    cout << endl << "Mediana Total: " << (unsigned char)medianaTotal << endl;

    return (unsigned char)medianaTotal;
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

void printImg(unsigned char** img, int w, int h)
{
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            printf("%d ", img[i][j]);
        }
        cout << endl;
    }
    cout << endl;
}

void printImg(unsigned char* img, int w, int h)
{
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            unsigned char val = img[AdaptCoords(i, j, w, h)];
            printf("%d ", val);
        }
        cout << endl;
    }
    cout << endl;
}