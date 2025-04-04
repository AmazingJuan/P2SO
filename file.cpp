#include "file.h"
#include <fstream>
#include <iostream>
#include "vcm.h"

File::File(const std::string &filename, bool recently_created = false)
{
    this -> filename = filename;
    if(recently_created){
        std::fstream archivo(filename, std::ios::out);
        std::fstream archivo2(filename + FILE_METADATA_EXTENSION, std::ios::out);
        json j;
        j["versions"] = "";
        j["blocks"] = "";
        metadata = j;
        std::ofstream meta(filename + FILE_METADATA_EXTENSION);
        if (meta.is_open()) {
            meta << j.dump(4);  // dump(4) para indentado limdo
            meta.close();
        }
        archivo.close();
        blocks_number = 0;
        loaded_block = new Block;
    }
    else{

    }
    buffer = new char[BUFFER_SIZE]();
    read_position = 0;
    write_position = 0;
    buffer_usage = 0;
}

File::~File()
{
    close();
}

void File::write(const char* content, std::streamsize stream_size)
{
    unsigned long left_streamsize;
    const char *left_content = fill_buffer(content, stream_size, left_streamsize);
    if(left_content != nullptr)
    {
        sync();
        delete[] buffer;
        buffer = new char[BLOCK_SIZE]();
        buffer_usage = 0;
        write(left_content, left_streamsize);
        version();
    }
}

void File::sync()
{
    std::ofstream archivo(filename, std::ios::app);
    archivo.write(buffer, buffer_usage);
    archivo.close();
}

void File::version()
{
    if (metadata["versions"].is_string()) {
        metadata["versions"] = json::array();
    }


    metadata["versions"].push_back({"hash", {{"starts_at", write_position}, {"offset", loaded_block->getBlock_usage()}}});
}

void File::close()
{
    if(buffer != nullptr) delete[] buffer;
    if(loaded_block != nullptr) delete loaded_block;
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

void File::dispose_block()
{
    delete loaded_block;
    loaded_block = new Block();
}

void File::select_block(unsigned int block_number)
{

}
