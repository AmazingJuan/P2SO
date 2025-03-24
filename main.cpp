#include "vcm.h"
#include <string>
int main()
{
    VCM sistema;
    File& archivos = sistema.create("archivo.txt");
    std::string hola(4097, 'a');
    archivos.write(hola.c_str(), 4097);
    archivos.write("54654", 5);
}
