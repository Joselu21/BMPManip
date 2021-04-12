//#include "../BMPManip/BMPManip.h"
#include <iostream>

using namespace std;

/*PROGRAMA QUE IMPRIME CARACTERES HASTA CIERTA POSICION EN ASM*/
/*
int main(int argc, char** argv) {

    const int tam = 1024;

    unsigned char* v = new unsigned char[tam];

    int pos = 0;
    char op = 's';

    /*
    if (i % 2 == 0) {
        v[i] = 'a';
    }
    if (i == 10) {
        v[i] = 'A';
    }
    if (i % 2 != 0) {
        v[i] = '0';
    }

    while (op != 'n')
    {
        cout << "PROGRAMA QUE IMPRIME CARACTERES HASTA CIERTA POSICION EN ASM" << "\n";
        cout << "Introduce la posicion del elemento que quieres imprimir: ";
        cin >> pos;

        for (int i = 0; i < tam; i++) {
            v[i] = (unsigned char)i;
        }

        int tamElem = sizeof(v[1]);
        int tamVector = sizeof(v[1]) * tam;

        int contador = 0;
        int i_esimo = 0;
        int dirMemoria = 0;

        //xmm0-7
        //st0-7

        _asm {

            mov edx, [contador]
            mov eax, v
            mov ecx, [tamElem]


            bucle:
            cmp edx, [pos]
                je finBucle
                mov ebx, 0
                mov ebx, [eax]
                mov[i_esimo], ebx
                mov contador, edx
                mov dirMemoria, eax
        }
        cout << (unsigned char)i_esimo << "\n";
        _asm {
            mov edx, [contador]
            mov eax, [dirMemoria]
            add edx, 1
            add eax, 1
            jmp bucle
            finBucle :

        }

        cout << "Terminado con exito";
        cout << "Desea continuar?(s/n): ";
        cin >> op;
        cout << endl;
    }

    return 0;
}
*/

/*PROGRAMA BUBLESORT Y CALCULO MEDIAN DE UN ARRAY DE CARACTERES, Y ADAPTCOORDS*/
int main()
{
    const int tam = 10;

    //unsigned char* v = new unsigned char[tam];
    unsigned char* v1 = new unsigned char[tam];
    unsigned int* v2 = new unsigned int[tam];

    cout << "PROGRAMA QUE ORDENA EL VECTOR (BUBBLE SORT)" << "\n";


    v1[0] = 'c';
    v1[1] = 'b';
    v1[2] = 'e';
    v1[3] = 'd';
    v1[4] = 'd';
    v1[5] = 'f';
    v1[6] = 'h';
    v1[7] = 'z';
    v1[8] = 'n';
    v1[9] = 'k';

    for (int i = 0;i < tam;i++)
    {
        v2[i] = v1[i];
    }


    //cout << "Carácter " << v1[0] << " en ASCII: " << v2[0];


    cout << "Vector inicial: ";
    for (int i = 0; i < tam; i++) {
        if (i != 0) {
            cout << ", ";
        }
        cout << v2[i];
    }


    int tamElem = sizeof(v2[1]);
    int tamVector = sizeof(v2[1]) * tam;

    int i = 2;
    int nj = tam - 1;
    int j = 0;

    unsigned int elem = 0;
    unsigned int elemSig = 0;
    unsigned int dirMemoria = 0;
    unsigned int dirMemoriaSig = 0;
    int intercambio = 0;
    unsigned int mediana = 0;
    unsigned int mediana2 = 0;
    unsigned int mediana3 = 0;
    unsigned int medianaTotal = 0;

    int x = 4;
    int y = 2;
    int Width = 5;
    int Height = 5;
    int pos = 0;


    _asm {
        mov edx, i
        mov ecx, j

        bucleExt :
        cmp edx, tam
            je finBucleExt
            mov eax, v2
            mov ecx, 0
            bucleInt :

            cmp ecx, nj
            je finBucleInt

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
            jmp bucleInt

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
            jmp bucleInt

            finBucleInt :
        inc edx
            jmp bucleExt

            finBucleExt :
        jmp Mediana

            Mediana :
        mov ecx, v2
            mov eax, tam
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
            mov medianaTotal, eax
            jmp finMediana

            finMediana :

    AdaptCoords:
        mov eax, x
            mov ebx, y
            cmp eax, 0
            jl superaLimites
            cmp ebx, 0
            jl superaLimites
            cmp eax, Height
            jge superaLimites
            cmp ebx, Width
            jge superaLimites

            mov eax, Width
            mul x
            sub Height, ebx
            sub Height, 1
            add eax, Height
            jmp finAdaptCoords

            superaLimites :
        mov eax, -1
            jmp finAdaptCoords

            finAdaptCoords :
        mov pos, eax



    }

    for (int i = 0;i < tam;i++)
    {
        v1[i] = v2[i];
    }


    cout << endl << "Vector final ordenado: ";
    for (int i = 0; i < tam; i++) {
        if (i != 0) {
            cout << ", ";
        }
        cout << v1[i];
    }

    cout << endl;
    cout << "Mediana 1: " << (unsigned char)mediana << endl;
    cout << "Mediana 2: " << (unsigned char)mediana2 << endl;
    cout << "Mediana 3: " << mediana3 << endl;
    cout << "Mediana Total: " << (unsigned char)medianaTotal << endl;

    cout << "AdaptCoords: " << pos << endl;

    cout << endl << "Terminado con exito";
    int n = 0;
    cin >> n;

    return 0;

}