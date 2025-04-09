#ifndef FILE_H
#define FILE_H
#include "block.h"
#include <string>
#include <vector>
#include "json.hpp"
#define BUFFER_SIZE BLOCK_SIZE
#define FILE_LATEST_CHECK_KEY "latest_version_shown"
#define FILE_VERSIONS_KEY "versions"
#define FILE_LATEST_SIZE "latest_size"
using json = nlohmann::json;

class File
{
public:
    File(const std::string &filename, json *system_meta, bool recently_created);
    ~File();
    void write(const char *content, std::streamsize stream_size);
    void read(char *content, std::streamsize stream_size);
    void move(const unsigned long desired_position);
    void close();
private:
    unsigned long byte_count;
    unsigned long actual_position;
    std::string filename;
    Block *loaded_block;
    unsigned long loaded_block_index;
    std::vector<std::string> blocks_hashes;
    char *buffer;
    unsigned short buffer_usage;
    json metadata;
    json *vcm_meta;
    const char *fill_buffer(const char *content, std::streamsize stream_size);
    void transfer_to_block(const char* content, const std::streamsize streamsize);
    void load_block(const std::string &hash);
    void dispose_block();
    void seekr(unsigned long byte_pos);
    unsigned int latest_version;
    void sync();
    void version();


};
#endif // FILE_H
