//#include "../BMPManip/BMPManip.h"
#include <iostream>

using namespace std;

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
    }*/

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

