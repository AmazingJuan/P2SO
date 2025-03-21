#ifndef FILE_H
#define FILE_H
#include "block.h"
#include <string>
#include <list>
class File
{
public:
    File(const std::string filename, bool recently_created);
    void write(const std::string content);
private:
    unsigned short blocks_number;
    std::string filename;
    std::list<Block*> blocks;

};

#endif // FILE_H
