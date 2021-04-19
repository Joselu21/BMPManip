#include <iostream>


using namespace std;

int main()
{
    const int tam = 25;
    int AdjacentValuesSize = 0;

    unsigned char* ImgChar = new unsigned char[tam];
    unsigned int* Img = new unsigned int[tam];
    unsigned int* AdjacentValues = new unsigned int[tam];

    cout << "***CALCULO DE LA MEDIANA DE UNA COMPONENTE MATRICIAL***" << endl;

    ImgChar[0] = 'a';
    ImgChar[1] = 'b';
    ImgChar[2] = 'c';
    ImgChar[3] = 'd';
    ImgChar[4] = 'e';
    ImgChar[5] = 'f';
    ImgChar[6] = 'g';
    ImgChar[7] = 'h';
    ImgChar[8] = 'i';
    ImgChar[9] = 'j';
    ImgChar[10] = 'k';
    ImgChar[11] = 'l';
    ImgChar[12] = 'm';
    ImgChar[13] = 'n';
    ImgChar[14] = 'o';
    ImgChar[15] = 'p';
    ImgChar[16] = 'q';
    ImgChar[17] = 'r';
    ImgChar[18] = 's';
    ImgChar[19] = 't';
    ImgChar[20] = 'u';
    ImgChar[21] = 'v';
    ImgChar[22] = 'w';
    ImgChar[23] = 'x';
    ImgChar[24] = 'y';

    for (int i = 0;i < tam;i++)
    {
        Img[i] = ImgChar[i];
    }


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

    int x = 0; //4
    int y = 0; //2
    int xIni = 0;
    int yIni = 0;
    int xFin = 0;
    int yFin = 0;
    int Width = 5;
    int Height = 5;
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

    cout << endl << "Terminado con exito";
    int n = 0;
    cin >> n;

    return 0;

}