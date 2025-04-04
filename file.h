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
    std::string current_block_hash;

    unsigned long read_position;
    unsigned long write_position;
    unsigned short blocks_number;
    std::string filename;
    Block *loaded_block;
    std::vector<std::string> blocks_hashes;
    char *buffer;
    unsigned short buffer_usage;
    json metadata;
    const char* fill_buffer(const char *content, std::streamsize stream_size, unsigned long &left_streamsize);
    void dispose_block();
    void select_block(unsigned int block_number);
    unsigned int latest_version;
    void generate_buffer_hash();
    void sync();
    void version();


};
#endif // FILE_H
