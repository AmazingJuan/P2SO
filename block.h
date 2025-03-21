#ifndef BLOCK_H
#define BLOCK_H
#define BLOCK_SIZE 4096
#include <string>
#include <array>
class Block
{
public:
    Block();
    Block(const std::string content);
    std::array<char, 4096> getContent() const;
    unsigned short fill(const std::string &content, bool &is_full, unsigned short &full_index);
    void setUsed_bytes(unsigned short newUsed_bytes);
    unsigned short getUsed_bytes() const;
private:
    unsigned short used_bytes;
    std::array<char, 4096> content;
};

#endif // BLOCK_H
