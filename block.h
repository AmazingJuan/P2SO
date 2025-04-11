#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>

#define BLOCK_SIZE 4096

class Block
{
public:
    Block();
    Block(const std::vector<char>& content, unsigned long offset);
    ~Block() = default;

    void setBlock_usage(unsigned short newBlock_usage);
    unsigned short getBlock_usage() const;

    const char* getContent() const;
    void setContent(const std::vector<char>& newContent);

    bool edit(const char* content, unsigned long offset);

    void setHash(const std::string &newHash);
    const std::string getHash() const;
    void generate_hash();

    unsigned short getIn_block_position() const;
    void setIn_block_position(unsigned short newIn_block_position);

private:
    unsigned short block_usage = 0;
    unsigned short in_block_position = 0;
    std::string hash;
    std::vector<char> content;
};

#endif // BLOCK_H
