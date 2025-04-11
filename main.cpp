#include "vcm.h"
#include <iostream>
int main()
{
    VCM sistema;
    File& archivo1 = sistema.open(R"(C:\Users\juanp\Downloads\Parcial2_MariaAcevedo_JuanAvendano.pdf)");
    char *nuevo = new char[289075];
    archivo1.read(nuevo, 289075);
    archivo1.close();
    File& archivo2 = sistema.open(R"(C:\Users\juanp\Downloads\Parcial2_MsssariaAcevedo_JuanAvendano.pdf)");
    archivo2.write(nuevo, 289075);
    archivo2.close();
    /*
    VCM sistema;
    File& archivos = sistema.create(R"(C:\Users\juanp\Downloads\archivos.txt)");
    std::string hola(8192, 'a');
    archivos.write(hola.c_str(), 8192);
    archivos.move(2000);
    std::string holi(10000, 'e');
    archivos.write(holi.c_str(), 10000);
    archivos.close();
    std:: cout << archivos.get_versions();

    /*
        VCM sistema;
        File& archivo1 = sistema.open(R"(C:\Users\juanp\Downloads\superpdf.pdf)");
        char *nuevo = new char[17166620]
        archivo1.read(nuevo, 17166620);
        archivo1.close();
        File& archivo2 = sistema.open(R"(C:\Users\juanp\Downloads\superpdf2.pdf)")
        archivo2.write(nuevo, 17166620);
        archivo2.close();
    */


}
