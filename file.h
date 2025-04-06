#ifndef FILE_H
#define FILE_H
#include "block.h"
#include <string>
#include <vector>
#include "json.hpp"
#define BUFFER_SIZE BLOCK_SIZE

using json = nlohmann::json;

class File
{
public:
    File(const std::string &filename, bool recently_created);
    ~File();
    void write(const char *content, std::streamsize stream_size);
    void close();
private:
    unsigned long current_position;
    unsigned long byte_count;
    std::string filename;
    Block *loaded_block;
    unsigned long loaded_block_index;
    std::vector<std::string> blocks_hashes;
    char *buffer;
    unsigned short buffer_usage;
    json metadata;
    const char *fill_buffer(const char *content, std::streamsize stream_size);
    void transfer_to_block(const char* content, const unsigned long streamsize);
    void dispose_block();
    void seekr(unsigned long byte_pos);
    unsigned int latest_version;
    void sync();
    void version();


};
#endif // FILE_H
