#ifndef BLOCK_H
#define BLOCK_H
#define BLOCK_SIZE 4096
#include <string>
class Block
{
public:
    Block();
    Block(const char* content, unsigned long offset, const std::string &hash);
    ~Block();
    void setBlock_usage(unsigned short newBlock_usage);
    unsigned short getBlock_usage() const;
    char *getContent() const;
    void setContent(char *newContent);
    bool edit(const char* content, unsigned long offset);
    void setHash(const std::string &newHash);
    const std::string getHash() const;
    void generate_hash();
    unsigned short getIn_block_position() const;
    void setIn_block_position(unsigned short newIn_block_position);

private:
    unsigned short block_usage;
    unsigned short in_block_position;
    std::string hash;
    char *content;
};

#endif // BLOCK_H
