#include "vcm.h"
#include <fstream>
#include <iostream>
VCM::VCM() {}

void VCM::create(const std::string filename)
{
    std::fstream archivo(filename + ".blocks", std::ios::out);
    std::fstream archivo2(filename + ".blocksmeta" , std::ios::out);
    archivo2.write("0,", 1);
    std::fstream archivo3(filename + ".versions", std::ios::out);
    opened_files[filename] = new File(filename, true);
}

void VCM::write(const std::string content, const std::string filename)
{
    opened_files[filename]->write(content);
}
