#include "file.h"

#include <fstream>

File::File(const std::string filename, bool recently_created = false)
{
    this -> filename = filename;
    if(recently_created){
        Block *first_block = new Block();
        blocks.push_back(first_block);
        std::ofstream archivo(filename + ".blocks", std::ios_base::binary);
        archivo.write(first_block->getContent().data(), 4096);
        first_block->setUsed_bytes(0);
        blocks_number = 1;
    }

}

void File::write(const std::string content)
{
    bool is_full = false;
    unsigned short full_index = 0;
    unsigned short filled_bytes = blocks.back()->fill(content, is_full, full_index);
    if(filled_bytes != 0){
        std::ofstream blocks_file(filename + ".blocks", std::ios_base::binary | std::ios_base::app);
        blocks_file.write(blocks.back()->getContent().data(), 4096);
    }
    else if(is_full){
        //
    }
}
