#include "file.h"
#include <fstream>
#include <iostream>
#include "vcm.h"
#include "utilities.h"
File::File(const std::string &filename, bool recently_created = false)
{
    this -> filename = filename;
    if(recently_created){
        std::fstream archivo(filename, std::ios::out);
        std::fstream archivo2(filename + FILE_METADATA_EXTENSION, std::ios::out);
        json j;
        j["versions"] = json::array();
        metadata = j;
        std::ofstream meta(filename + FILE_METADATA_EXTENSION);
        if (meta.is_open()) {
            meta << j.dump(4);  // dump(4) para indentado limdo
            meta.close();
        }
        archivo.close();
        blocks_number = 0;
        loaded_block = new Block;
        latest_version = 0;
    }
    else{

    }
    buffer = new char[BUFFER_SIZE]();
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
    if(left_content != nullptr || buffer_usage == BLOCK_SIZE)
    {
        generate_buffer_hash();
        sync();
        version();
        delete[] buffer;
        buffer = new char[BLOCK_SIZE]();
        buffer_usage = 0;
        write(left_content, left_streamsize);
    }
}

void File::sync()
{
    std::ofstream archivo(filename, std::ios::app);
    archivo.write(buffer, buffer_usage);
    archivo.close();
    std::fstream vcm_data(VCM_META_FILENAME, std::ios::in|std::ios::out);
    json j = json::parse(vcm_data);
    std::ofstream vcm_blocks(VCM_BLOCKS_FILENAME, std::ios::app);
    if (!hash_exists(j, current_block_hash)){
        unsigned int pos = vcm_blocks.tellp();
        j["blocks"].push_back({{"hash", current_block_hash}, {"pos", pos}, {"offset", buffer_usage}});
        save_json(j, VCM_META_FILENAME);
        vcm_blocks.write(buffer, buffer_usage);
        vcm_blocks.close();
    }

}

void File::version()
{
    blocks_hashes.push_back(current_block_hash);
    latest_version++;
    metadata["versions"].push_back({{"name", "v" + std::to_string(latest_version)}, {"blocks", blocks_hashes}, {"timestamp", get_time_stamp()}});

    save_json(metadata, filename + FILE_METADATA_EXTENSION);
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

void File::generate_buffer_hash()
{
    current_block_hash = md5(buffer);
}
