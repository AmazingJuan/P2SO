#ifndef BLOCK_H
#define BLOCK_H
#include <ios>
#define BLOCK_SIZE 4096
class Block
{
public:
    Block();
    void setBlock_usage(unsigned short newBlock_usage);

    unsigned short getBlock_usage() const;

    char *getContent() const;

    void setContent(char *newContent);

    bool edit(char* content, unsigned long position, unsigned long offset);
private:
    unsigned short block_usage;
    char *content;
};

#endif // BLOCK_H
