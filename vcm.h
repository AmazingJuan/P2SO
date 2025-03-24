#ifndef VCM_H
#define VCM_H
#include "file.h"
#include <string>
#include <unordered_map>

#define HASH_EXTENSION ".block"
#define METADATA_EXTENSION ".meta"

class VCM
{
public:
    VCM();
    File &open();
    File &create(const std::string filename);
    void read();
private:
    std::unordered_map<std::string, File*> opened_files;
};

#endif // VCM_H
