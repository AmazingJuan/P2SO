#include "block.h"

Block::Block() {
    std::fill(std::begin(content), std::end(content), 0);
}

unsigned short Block::fill(const std::string &content, bool &is_full, unsigned short &full_index)
{
    if(used_bytes < 4096){
        unsigned short initial_usage = used_bytes;
        unsigned short i = 1;
        for(;used_bytes < 4096 && i <= content.length(); i++){
            this->content[initial_usage + i - 1]= content[i-1];
            used_bytes++;
        }
        if(used_bytes == 4096 && i != content.length()){
            is_full = true;
            full_index = i;
        }
        return i;
    }
    else return 0;
}

void Block::setUsed_bytes(unsigned short newUsed_bytes)
{
    used_bytes = newUsed_bytes;
}

unsigned short Block::getUsed_bytes() const
{
    return used_bytes;
}

std::array<char, 4096> Block::getContent() const
{
    return content;
}
