#ifndef VCM_H
#define VCM_H
#include "file.h"
#include <string>
#include <unordered_map>
class VCM
{
public:
    VCM();
    void open();
    void create(const std::string filename);
    void write(const std::string content, const std::string filename);
    void read();
private:
    std::unordered_map<std::string, File*> opened_files;

};

#endif // VCM_H
