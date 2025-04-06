#ifndef BLOCK_H
#define BLOCK_H
#define BLOCK_SIZE 4096
#include <string>
class Block
{
public:
    Block();
    ~Block();
    void setBlock_usage(unsigned short newBlock_usage);
    unsigned short getBlock_usage() const;
    char *getContent() const;
    void setContent(char *newContent);
    bool edit(const char* content, unsigned long offset);
    void setHash(const std::string &newHash);
    const std::string getHash() const;
    void generate_hash();
private:
    unsigned short block_usage;
    std::string hash;
    char *content;
};

#endif // BLOCK_H
