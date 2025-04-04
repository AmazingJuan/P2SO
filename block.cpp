#include "block.h"
#include "utilities.h"
Block::Block() {
    content = new char[BLOCK_SIZE]();
    block_usage = 0;
}

Block::~Block()
{
    if(content != nullptr) delete [] content;
}

void Block::setBlock_usage(unsigned short newBlock_usage)
{
    block_usage = newBlock_usage;
}

unsigned short Block::getBlock_usage() const
{
    return block_usage;
}

char *Block::getContent() const
{
    return content;
}

void Block::setContent(char *newContent)
{
    content = newContent;
}

bool Block::edit(char *content, unsigned long position, unsigned long offset)
{
    unsigned long i;
    for(i = 0; i != offset - 1 && position + i != BLOCK_SIZE - 1; i++){
        this->content[position + i] = content[i];
    }

    if(i == offset - 1){
        return true;
    }
    else{
        return false;
    }
}

void Block::setHash(const std::string &newHash)
{
    hash = newHash;
}

const std::string Block::getHash() const
{
    return hash;
}

void Block::generate_hash()
{
    md5(content);
}
