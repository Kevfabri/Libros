#include "Validaciones.hpp"
#include <iostream>

#include <termios.h> 
#include <unistd.h> 
#include <stdio.h>
int getch()
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

int Validaciones::leerInt()
{
    const int INT_TAM = 12;
    char ingreso[INT_TAM] = "";
    char c;

    int pos = 0;
    do
    {
        c = getch();
        if (pos != INT_TAM - 1)
        {
            if (('0' <= c && c <= '9') || (c == '-' && pos == 0))
            {
                putchar(c); //
                ingreso[pos] = c;
                ingreso[++pos] = '\0';
            }
        }

        if ((c == '\b' || c == 127) && pos != 0) {
            putchar('\b');
            putchar(' ');
            putchar('\b');
            ingreso[--pos] = '\0';
        }
    } while ((c != '\n' && c != '\r') || pos == 0);

    putchar('\n');
    return atoi(ingreso);
}

float Validaciones::leerFloat()
{
    const int FLOAT_TAM = 42;
    char ingreso[FLOAT_TAM] = "";
    char c;
    bool tienePunto;

    int pos = 0;
    do
    {
        c = getch();
        if (pos != FLOAT_TAM - 1)
        {
            if (('0' <= c && c <= '9') || (c == '-' && pos == 0))
            {
                putchar(c); //
                ingreso[pos] = c;
                ingreso[++pos] = '\0';
            }

            if (!tienePunto && (c == '.' || c == ',')) {
                c = '.';
                putchar(c); // 1
                ingreso[pos] = c;
                ingreso[++pos] = '\0';

                tienePunto = true;
            }
        }

        if ((c == '\b' || c == 127) && pos != 0) {
            putchar('\b');
            putchar(' ');
            putchar('\b');

            if (ingreso[--pos] == '.')
                tienePunto = false;
            ingreso[pos] = '\0';
        }
    } while ((c != '\n' && c != '\r') || pos == 0);

    putchar('\n');
    return atof(ingreso);
}

double Validaciones::leerDouble()
{
    const int DOUBLE_TAM = 42;
    char ingreso[DOUBLE_TAM] = "";
    char c;
    bool tienePunto;

    int pos = 0;
    do
    {
        c = getch();
        if (pos != DOUBLE_TAM - 1)
        {
            if (('0' <= c && c <= '9') || (c == '-' && pos == 0))
            {
                putchar(c); //
                ingreso[pos] = c;
                ingreso[++pos] = '\0';
            }

            if (!tienePunto && (c == '.' || c == ',')) {
                c = '.';
                putchar(c); // 1
                ingreso[pos] = c;
                ingreso[++pos] = '\0';

                tienePunto = true;
            }
        }

        if ((c == '\b' || c == 127) && pos != 0) {
            putchar('\b');
            putchar(' ');
            putchar('\b');

            if (ingreso[--pos] == '.')
                tienePunto = false;
            ingreso[pos] = '\0';
        }
    } while ((c != '\n' && c != '\r') || pos == 0);

    putchar('\n');
    return atof(ingreso);
}

char Validaciones::leerLetra()
{
    char c;

    do
    {
        c = getch();

        if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
        {
            putchar(c);
        }

        if ((c == '\b' || c == 127)) {
            putchar('\b');
            putchar(' ');
            putchar('\b');
            c = '\0';

        }
    } while ((c != '\n' && c != '\r'));

    return c;
}

std::string Validaciones::leerPalabra()
{
    char c;
    std::string saveStr = "";

    do
    {
        c = getch();

        if ('A' <= c && c <= 'Z' || 'a' <= c && c <= 'z')
        {
            putchar(c); // 1
            saveStr.push_back(c);
        }

        if ((c == '\b' || c == 127) && !saveStr.empty()) {
            putchar('\b');
            putchar(' ');
            putchar('\b');
            saveStr.pop_back();
        }

    } while ((c != '\n' && c != '\r')); // 2

    return saveStr;
}

std::string Validaciones::leerCedula()
{
    const int CED_TAM = 11;
    char ingreso[CED_TAM] = "";
    char c;

    int pos = 0;
    do
    {
        c = getch();
        if (pos != CED_TAM - 1)
        {
            if (('0' <= c && c <= '9') || (c == '-' && pos == 0))
            {
                putchar(c); //
                ingreso[pos] = c;
                ingreso[++pos] = '\0';
            }
        }

        if ((c == '\b' || c == 127) && pos != 0) {
            putchar('\b');
            putchar(' ');
            putchar('\b');
            ingreso[--pos] = '\0';
        }
    } while ((c != '\n' && c != '\r') || pos == 0);

    putchar('\n');
    return std::string(ingreso);
}

std::string Validaciones::leerNumero()
{
    char c;
    std::string saveStr = "";

    do
    {
        c = getch();

        if ('0' <= c && c <= '9')
        {
            putchar(c); // 1
            saveStr.push_back(c);
        }

        if ((c == '\b' || c == 127) && !saveStr.empty()) {
            putchar('\b');
            putchar(' ');
            putchar('\b');
            saveStr.pop_back();
        }

    } while ((c != '\n' && c != '\r')); // 2

    return saveStr;
}

bool Validaciones::validarCedula(std::string cedula)
{
    int verif[10], num, i;
    auto char2int = [](char c) {return (int)c - 48; };

    std::cout << std::endl;

    if (cedula.length() != 10) {
        std::cout << "Longitud invalida. " << std::endl;
        return 0;
    }

    if (char2int(cedula.at(2)) < 0 || char2int(cedula.at(2)) > 6) {
        printf("Error en el tercer digito.\n");
        return 0;
    }
    if (char2int(cedula.at(0)) < 0 || char2int(cedula.at(0)) > 2) {
        printf("Error en el codigo de provincia.\n");
        return 0;
    }
    if (char2int(cedula.at(0)) == 2 && char2int(cedula.at(1)) < 1 || char2int(cedula.at(1)) - 48 > 4) {
        printf("Error en el codigo de provincia.\n");
        return 0;
    }
    for (i = 0; i <= 8; i++) {
        if (i % 2 == 0) {
            verif[i] = char2int(cedula.at(i)) * 2;
            if (verif[i] >= 10) {
                verif[i] = verif[i] - 9;

            }
        }
        else {
            verif[i] = char2int(cedula.at(i)) * 1;
            if (verif[i] >= 10) {
                verif[i] = verif[i] - 9;

            }
        }
    }
    verif[9] = 90 - [verif]() {
        int i, suma = 0;
        for (i = 0; i <= 8; i++) {
            suma = suma + verif[i];
        }
        return suma;
        }();

    num = verif[9] % 10;

    if (char2int(cedula.at(9)) == num) {
        printf("Digito verificador obtenido: %d.\n", num);
        return 1;
    }
    else {
        printf("Digito verificador obtenido: %d. Error en el digito verificador.\n", num);
        return 0;
    }
}
