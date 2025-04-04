#include "block.h"

Block::Block() {
    content = new char[BLOCK_SIZE]();
    block_usage = 0;
}

Block::~Block()
{

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
