#include "vcm.h"
#include <fstream>
#include <iostream>
VCM::VCM() {}

File& VCM::create(const std::string filename)
{
    opened_files[filename] = new File(filename, true);
    return *opened_files[filename];
}
