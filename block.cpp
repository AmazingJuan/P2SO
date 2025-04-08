#include "block.h"
#include "utilities.h"
Block::Block() {
    content = new char[BLOCK_SIZE]();
    content[BLOCK_SIZE] = 0;
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

bool Block::edit(const char *content, unsigned long offset)
{
    unsigned long i;
    for(i = 0; i != offset && block_usage != BLOCK_SIZE; i++){
        this->content[block_usage] = content[i];
        block_usage++;
    }

    if(i == offset){
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
    hash = sha256(content);
}
