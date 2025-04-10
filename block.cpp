#include "block.h"
#include "utilities.h"
Block::Block() {
    content = new char[BLOCK_SIZE]();
    content[BLOCK_SIZE] = 0;
    block_usage = 0;
    in_block_position = 0;
}

Block::Block(const char *content, unsigned long offset)
{
    this->content = new char[BLOCK_SIZE]();
    this->content[BLOCK_SIZE] = 0;
    block_usage = offset;
    for(unsigned long i = 0; i < offset; i++){
        this->content[i] = content[i];
    }
    in_block_position = 0;
    generate_hash();
}

Block::~Block()
{
    if(block_usage != 0) delete[] content;
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
    for(i = 0; i != offset && in_block_position != BLOCK_SIZE; i++){
        this->content[in_block_position] = content[i];
        if(in_block_position  == block_usage){
            block_usage++;
        }
        in_block_position++;
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

unsigned short Block::getIn_block_position() const
{
    return in_block_position;
}

void Block::setIn_block_position(unsigned short newIn_block_position)
{
    in_block_position = newIn_block_position;
}
