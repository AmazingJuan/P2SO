#ifndef VCM_H
#define VCM_H
#include "file.h"
#include <string>
#include <unordered_map>
#include <fstream>
#define VCM_BLOCKS_FILENAME "VCM_BLOCKS.SO"
#define VCM_META_FILENAME "VCM_META.SO"
#define FILE_METADATA_EXTENSION ".SOMETA"

class VCM
{
public:
    VCM();
    File open();
    File create(const std::string filename);
private:
    std::unordered_map<std::string, File*> opened_files;
    json metadata;
};

#endif // VCM_H
