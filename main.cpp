#include "vcm.h"
#include <iostream>
int main()
{
    VCM sistema;
    /*
    File archivos = sistema.open("archivoss.txt");
    archivos.move(4096);
    */
    File archivos = sistema.create("archivosppssssss.txt");
    std::string hola(4095, 'a');
    hola += "e";
    std::cout << hola[4095];
    archivos.write(hola.c_str(), 4096);
    archivos.move(4000);
    std::string holi(100, 'e');
    archivos.write(holi.c_str(), 100);
}
