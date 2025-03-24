#include "file.h"

#include <fstream>

File::File(const std::string &filename, bool recently_created = false)
{
    this -> filename = filename;
    if(recently_created){
        std::fstream archivo(filename, std::ios::out);
        archivo.close();
        blocks_number = 0;
        file_reference = std::fstream(filename, std::ios::out | std::ios::in | std::ios_base::binary);
        loaded_block = new Block;
    }
    else{

    }
    buffer = new char[BUFFER_SIZE]();
    read_position = 0;
    write_position = 0;
    buffer_usage = 0;

}

void File::write(const char* content, std::streamsize stream_size)
{
    auto previous_usage = buffer_usage;
    unsigned long left_streamsize;
    const char *left_content = fill_buffer(content, stream_size, left_streamsize);
    if(left_content == nullptr)
    {
        loaded_block->edit(buffer, write_position, stream_size);
    }
    else{

        //tin
        write(left_content, left_streamsize);
    }
}

const char *File::fill_buffer(const char *content, std::streamsize stream_size, unsigned long &left_streamsize)
{
    unsigned int cont = 0;
    while(buffer_usage != BUFFER_SIZE && cont != stream_size){
        buffer[buffer_usage] = content[cont];
        buffer_usage++;
        cont++;
        write_position++;
    }

    if(buffer_usage == BUFFER_SIZE && cont != stream_size){
        left_streamsize = stream_size - cont;
        return &content[cont];
    }
    else{
        return nullptr;
    }

}
