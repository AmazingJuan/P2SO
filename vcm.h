#ifndef VCM_H
#define VCM_H
#include "file.h"
#include <string>
#include <unordered_map>
#include <fstream>
#define VCM_BLOCKS_FILENAME "VCM_BLOCKS.SO"
#define VCM_META_FILENAME "VCM_META.SO"
#define FILE_METADATA_EXTENSION ".SOMETA"
#define META_FILES_KEY "files"
#define META_BLOCKS_KEY "blocks"

class VCM
{
public:
    VCM();
    ~VCM();
    void extracted(json &file_meta, std::ofstream &phys_file,
                   std::ifstream &blocks_file);
    File& open(const std::string &filename);
    File& create(const std::string &filename);
private:
    std::unordered_map<std::string, File*> opened_files;
    json *metadata;
};

#endif // VCM_H
