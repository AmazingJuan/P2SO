#include "block.h"
#include "utilities.h"
#include <algorithm>

Block::Block()
{
    content.resize(BLOCK_SIZE, 0);  // Inicializa con ceros
    block_usage = 0;
    in_block_position = 0;
}

Block::Block(const std::vector<char>& content, unsigned long offset)
{
    this->content.resize(BLOCK_SIZE, 0);
    std::copy_n(content.begin(), std::min(offset, static_cast<unsigned long>(BLOCK_SIZE)), this->content.begin());
    block_usage = static_cast<unsigned short>(offset);
    in_block_position = 0;
    generate_hash();
}

void Block::setBlock_usage(unsigned short newBlock_usage)
{
    block_usage = newBlock_usage;
}

unsigned short Block::getBlock_usage() const
{
    return block_usage;
}

const char* Block::getContent() const
{
    return content.data();
}

void Block::setContent(const std::vector<char>& newContent)
{
    content = newContent;
    block_usage = static_cast<unsigned short>(std::min(static_cast<size_t>(BLOCK_SIZE), newContent.size()));
}

bool Block::edit(const char* input, unsigned long offset)
{
    unsigned long i = 0;
    while (i < offset && in_block_position < BLOCK_SIZE) {
        content[in_block_position] = input[i];
        if (in_block_position == block_usage) {
            block_usage++;
        }
        in_block_position++;
        i++;
    }

    return i == offset;
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
    hash = sha256(content.data(), block_usage);
}

unsigned short Block::getIn_block_position() const
{
    return in_block_position;
}

void Block::setIn_block_position(unsigned short newIn_block_position)
{
    in_block_position = newIn_block_position;
}
