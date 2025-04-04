#include "vcm.h"
#include <fstream>
int main()
{
    VCM sistema;
    File archivos = sistema.create("archivo.txt");
    std::string hola(8192, 'a');
    archivos.write(hola.c_str(), 8192);
}
