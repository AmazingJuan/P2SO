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
    if(buffer != nullptr) delete[] buffer;
    if(loaded_block != nullptr) delete loaded_block;

}

void File::write(const char* content, std::streamsize stream_size)
{
    unsigned long left_buffer_streamsize = stream_size;
    const char *left_buffer_content = fill_buffer(content, stream_size);
    left_buffer_streamsize -= buffer_usage;

    if(left_buffer_streamsize > 0)
    {
        loaded_block->edit("aa", 2);
        transfer_to_block(buffer, buffer_usage);
        delete[] buffer;
        buffer = new char[BLOCK_SIZE]();
        buffer_usage = 0;
        write(left_buffer_content, left_buffer_streamsize);
    }
}

void File::sync()
{
    std::ofstream archivo(filename, std::ios::app);
    archivo.write(loaded_block->getContent(), loaded_block->getBlock_usage());
    archivo.close();

    std::fstream vcm_data(VCM_META_FILENAME, std::ios::in|std::ios::out|std::ios::ate);
    json j;
    if(vcm_data.tellg() != 0){
        vcm_data.seekg(0);
        j = json::parse(vcm_data);
    }
    std::ofstream vcm_blocks(VCM_BLOCKS_FILENAME, std::ios::app);
    std::string current_block_hash = loaded_block->getHash();
    if (!j.contains(current_block_hash)){
        unsigned int pos = vcm_blocks.tellp();
        j[current_block_hash]= {{"pos", pos}, {"offset", loaded_block->getBlock_usage()}};
        save_json(j, VCM_META_FILENAME);
        vcm_blocks.write(loaded_block->getContent(), loaded_block->getBlock_usage());
        vcm_blocks.close();
    }
    vcm_data.close();
}

void File::version()
{
    blocks_hashes.push_back(loaded_block->getHash());
    latest_version++;
    metadata["versions"].insert(metadata["versions"].begin(), json{{"name", "v" + std::to_string(latest_version)}, {"blocks", blocks_hashes}, {"timestamp", get_time_stamp()}});
    save_json(metadata, filename + FILE_METADATA_EXTENSION);
}

void File::close()
{
}

const char *File::fill_buffer(const char *content, std::streamsize stream_size)
{
    unsigned int cont = 0;
    while(buffer_usage != BUFFER_SIZE && cont != stream_size){
        buffer[buffer_usage] = content[cont];
        buffer_usage++;
        cont++;
        current_position++;
    }

    if(buffer_usage == BUFFER_SIZE && cont != stream_size){
        return &content[cont];
    }
    else{
        return nullptr;
    }

}

void File::transfer_to_block(const char* content, const unsigned long streamsize)
{

    const char *left_block_content = content;
    unsigned long left_block_streamsize = streamsize;
    unsigned long previous_block_usage = loaded_block->getBlock_usage();
    while(left_block_content != nullptr){
        loaded_block->edit(left_block_content, left_block_streamsize);
        if(loaded_block->getBlock_usage() == BLOCK_SIZE){
            loaded_block->generate_hash();
            sync();
            version();
            left_block_streamsize -= loaded_block->getBlock_usage() - previous_block_usage;
            left_block_content = &content[loaded_block->getBlock_usage() - previous_block_usage];
            delete loaded_block;
            loaded_block = new Block();
            if(left_block_streamsize == 0) left_block_content = nullptr;
        }
        else{
            left_block_content = nullptr;
        }
    }
}

void File::dispose_block()
{
    delete loaded_block;
    loaded_block = new Block();
}

void File::seekr(unsigned long byte_pos)
{
    unsigned long block_count = 0;
    std::ifstream archivo(VCM_META_FILENAME);
    json VCM_META = json::parse(archivo);
    for(std::string block : blocks_hashes){


    }
}
