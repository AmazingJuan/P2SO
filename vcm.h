#ifndef VCM_H
#define VCM_H
#include "file.h"
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#define VCM_BLOCKS_FILENAME "VCM_BLOCKS.SO"
#define VCM_META_FILENAME "VCM_META.SO"
#define FILE_METADATA_EXTENSION ".SOMETA"
#define META_FILES_KEY "files"
#define META_BLOCKS_KEY "blocks"
namespace fs = std::filesystem;
inline fs::path get_user_home_path() {
#ifdef _WIN32
    return fs::path(std::getenv("USERPROFILE")); // Windows
#else
    return fs::path(std::getenv("HOME")); // Unix
#endif
}
class VCM
{
public:
    VCM();
    ~VCM();
    File& open(const std::string &filename);
    File& create(const std::string &filename);
    unsigned long memory_usage();
private:
    std::unordered_map<std::string, File*> opened_files;
    json *metadata;
};

#endif // VCM_H
