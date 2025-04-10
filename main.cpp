#include "vcm.h"
#include <iostream>
int main()
{
    VCM sistema;
    File archivos = sistema.open("archivosppsssssss.txt");
    /*archivos.move(11999);
    std::string holi(4096, 'a');
    archivos.write(holi.c_str(), 4096);

    /*
    File archivos = sistema.open("archivoss.txt");


    std::string holi(8192, 'a');
    archivos.write(holi.c_str(), 8192);

    std:: cout << archivos.get_versions();
    archivos.move(2000);
    std::string hola(10000, 'e');
    archivos.write(hola.c_str(), 10000);
    std:: cout << archivos.get_versions();
    */
}
