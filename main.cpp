#include "vcm.h"
#include <iostream>
int main()
{
    VCM sistema;
    File archivos = sistema.create("archivoss.txt");
    std::string hola(4095, 'a');
    hola += "e";
    std::cout << hola[4095];
    archivos.write(hola.c_str(), 4096);
    std::string holi(4096, 'a');
    archivos.write(holi.c_str(), 4096);
}
