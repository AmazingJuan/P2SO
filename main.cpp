#include "vcm.h"
#include <iostream>
int main()
{


    VCM sistema;

    File& archivos = sistema.open("holi.exe");
    /*
    std:: cout << archivos.get_versions();
    archivos.move(2000);
    std::string hola(10000, 'e');
    archivos.write(hola.c_str(), 10000);
    std:: cout << archivos.get_versions();
    /*
     * File& archivos = sistema.open("lghub_installer.exe");
    std::ifstream archivo("lghub_installer.txt" , std::ios_base::binary);
    char *hola = new char[58976136];
    archivo.read(hola, 58976136);
    std::ofstream archivo2("hi2.txt", std::ios_base::binary);
    archivo2.write(hola, 58171392);
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
