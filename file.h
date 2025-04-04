#ifndef FILE_H
#define FILE_H
#include "block.h"
#include <string>
#include "json.hpp"
#define BUFFER_SIZE BLOCK_SIZE

using json = nlohmann::json;

class File
{
public:
    File(const std::string &filename, bool recently_created);
    void write(const char *content, std::streamsize stream_size);
    void sync();
    void version();
private:
    unsigned long read_position;
    unsigned long write_position;
    unsigned short blocks_number;
    std::string filename;
    Block *loaded_block;
    char *buffer;
    unsigned short buffer_usage;
    json metadata;
    const char* fill_buffer(const char *content, std::streamsize stream_size, unsigned long &left_streamsize);
    void dispose_block();
    void select_block(unsigned int block_number);

};
#endif // FILE_H
