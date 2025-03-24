#ifndef FILE_H
#define FILE_H
#include "block.h"
#include <string>
#include <fstream>
#define BUFFER_SIZE BLOCK_SIZE

class File
{
public:
    File(const std::string &filename, bool recently_created);
    void write(const char *content, std::streamsize stream_size);
private:
    unsigned long read_position;
    unsigned long write_position;
    unsigned short blocks_number;
    std::string filename;
    Block *loaded_block;
    char *buffer;
    unsigned short buffer_usage;
    std::fstream file_reference;

    const char* fill_buffer(const char *content, std::streamsize stream_size, unsigned long &left_streamsize);

};
#endif // FILE_H
