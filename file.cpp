#include "file.h"
#include <fstream>
#include <iostream>
#include "vcm.h"
#include "utilities.h"
#include <format>
File::File(const std::string &filename, json *system_meta, bool recently_created = false)
{
    this -> filename = filename;
    if(recently_created){
        std::ofstream archivo(filename);
        archivo.close();
        std::fstream archivo2(filename + FILE_METADATA_EXTENSION, std::ios::out);
        archivo2.close();
        json j;
        j[FILE_VERSIONS_KEY] = json::array();
        j[FILE_LATEST_CHECK_KEY] = true;
        j[FILE_LATEST_VERSION] = 0;
        metadata = j;
        save_json(metadata, filename + FILE_METADATA_EXTENSION);
        loaded_block = new Block;
        latest_version = 0;
        loaded_block_index = 0;
    }
    else{
        std::ifstream meta_file(filename + FILE_METADATA_EXTENSION);
        json file_meta = json::parse(meta_file);
        meta_file.close();
        metadata = file_meta;
        std::ifstream vcm_blocks(VCM_BLOCKS_FILENAME, std::ios_base::binary);
        for(std::string block_hash: file_meta[FILE_VERSIONS_KEY][0]["blocks"] ){
            blocks_hashes.push_back(block_hash);
        }
        if(blocks_hashes.size() > 0){
            std::string hash = blocks_hashes[0];
            unsigned long position = (*system_meta)[META_BLOCKS_KEY][hash]["pos"];
            std::streamsize offset = (*system_meta)[META_BLOCKS_KEY][hash]["offset"];
            char *content = new char[offset]();
            vcm_blocks.seekg(position);
            vcm_blocks.read(content, offset);
            loaded_block = new Block(content, offset);
            loaded_block_index = 0;
            delete[] content;
        }
        else{
            loaded_block = new Block();
        }
    }
    current_version = 0;
    blocks_hashes.push_back("blank");
    actual_position = 0;
    buffer = new char[BUFFER_SIZE]();
    buffer_usage = 0;
    this ->vcm_meta = system_meta;
    latest_version = metadata[FILE_LATEST_VERSION];
    if(!metadata[FILE_LATEST_CHECK_KEY]){
        change_version(latest_version);
    }
}

File::~File()
{
    close();
    if(buffer != nullptr) delete[] buffer;
    if(loaded_block != nullptr) delete loaded_block;
}

void File::write(const char* content, std::streamsize stream_size)
{
    unsigned long left_buffer_streamsize = stream_size;
    unsigned long previous_buffer = buffer_usage;
    const char *left_buffer_content = fill_buffer(content, stream_size);
    left_buffer_streamsize -= buffer_usage - previous_buffer;

    if(left_buffer_streamsize > 0 || buffer_usage == BUFFER_SIZE)
    {
        transfer_to_block(buffer, buffer_usage);
        delete[] buffer;
        buffer = new char[BLOCK_SIZE]();
        buffer_usage = 0;
        if(left_buffer_streamsize > 0){
            write(left_buffer_content, left_buffer_streamsize);
        }
    }
}

void File::sync()
{
    std::fstream archivo(filename, std::ios::in | std::ios::out);
    archivo.seekp(actual_position - loaded_block->getIn_block_position());
    archivo.write(loaded_block->getContent(), loaded_block->getBlock_usage());
    archivo.close();
    std::ofstream vcm_blocks(VCM_BLOCKS_FILENAME, std::ios::app);
    std::string current_block_hash = loaded_block->getHash();
    if ((*vcm_meta)[META_BLOCKS_KEY].is_null() || !(*vcm_meta)[META_BLOCKS_KEY].contains(current_block_hash)){
        vcm_blocks.seekp(0, std::ios::end);
        unsigned int pos = vcm_blocks.tellp();
        (*vcm_meta)[META_BLOCKS_KEY][current_block_hash] =  {{"offset", loaded_block->getBlock_usage()}, {"pos", pos}};
        save_json(*vcm_meta, VCM_META_FILENAME);
        vcm_blocks.write(loaded_block->getContent(), loaded_block->getBlock_usage());
        vcm_blocks.close();
    }
}

void File::version()
{
    latest_version++;
    current_version = latest_version;
    metadata[FILE_LATEST_VERSION] = latest_version;
    auto aux = json::array();
    for(std::string hash : blocks_hashes){
        if(hash != "blank"){
            aux.push_back(hash);
        }
    }
    metadata[FILE_VERSIONS_KEY].insert(metadata[FILE_VERSIONS_KEY].begin(), json{{"id", latest_version}, {"blocks", aux}, {"timestamp", get_time_stamp()}});
    save_json(metadata, filename + FILE_METADATA_EXTENSION);
}

void File::change_version(unsigned long id)
{
    if(id != current_version && id <= latest_version){
        current_version = id;
        auto blocks = metadata[FILE_VERSIONS_KEY][latest_version - id]["blocks"];
        std::ofstream aux_file(filename);
        std::ifstream blocks_file(VCM_BLOCKS_FILENAME, std::ios_base::binary);
        for(std::string hash : blocks){
            auto info = (*vcm_meta)["blocks"][hash];
            unsigned long offset = info["offset"];
            unsigned long position = info["pos"];
            char *content = new char[offset];
            blocks_file.seekg(position);
            blocks_file.read(content, offset);
            aux_file.write(content, offset);
            delete [] content;
        }
        if(current_version != latest_version){
            metadata[FILE_LATEST_CHECK_KEY] = false;
            save_json(metadata, filename + FILE_METADATA_EXTENSION);
        }
        else if(!metadata[FILE_LATEST_CHECK_KEY]){
            metadata[FILE_LATEST_CHECK_KEY] = true;
            save_json(metadata, filename + FILE_METADATA_EXTENSION);
        }
        blocks_file.close();
        aux_file.close();
    }
}

std::string File::get_versions()
{
    std::string aux = "";
    for (const auto& version : metadata[FILE_VERSIONS_KEY]) {
        int id = version["id"];
        std::string timestamp = version["timestamp"];
        aux += std::format("Version {:<3} |  Timestamp: {}\n", id, timestamp);
    }
    return aux;
}

void File::move(const unsigned long desired_position)
{
    unsigned long position_counter = 0;
    unsigned long counter = 0;
    for(std::string hash : blocks_hashes){
        if(hash != "blank"){
            std::streamsize offset = (*vcm_meta)[META_BLOCKS_KEY][hash]["offset"];
            if(desired_position >= position_counter && desired_position < position_counter + offset){
                if(hash != loaded_block->getHash()){
                    delete loaded_block;
                    load_block(hash);
                }
                actual_position = desired_position;
                loaded_block->setIn_block_position((desired_position - position_counter)%offset);
                loaded_block_index = counter;
                delete []buffer;
                buffer = new char[BUFFER_SIZE];
                buffer_usage = 0;
                break;
            }
            else{
                position_counter += offset;
            }
            counter++;
        }
    }
}

void File::close()
{
    if(buffer_usage > 0){
        transfer_to_block(buffer, buffer_usage);
    }
}

const char *File::fill_buffer(const char *content, std::streamsize stream_size)
{
    unsigned int cont = 0;
    while(buffer_usage != BUFFER_SIZE && cont != stream_size){
        buffer[buffer_usage] = content[cont];
        buffer_usage++;
        cont++;
    }

    if(buffer_usage == BUFFER_SIZE && cont != stream_size){
        return &content[cont];
    }
    else{
        return nullptr;
    }

}

void File::transfer_to_block(const char* content, const std::streamsize streamsize)
{
    const char *left_block_content = content;
    unsigned long left_block_streamsize = streamsize;
    unsigned long previous_block_usage = loaded_block->getIn_block_position();
    while(left_block_streamsize != 0){
        loaded_block->edit(left_block_content, left_block_streamsize);
        loaded_block->generate_hash();
        blocks_hashes[loaded_block_index] = loaded_block->getHash();
        left_block_streamsize -= loaded_block->getIn_block_position() - previous_block_usage;
        left_block_content = &left_block_content[loaded_block->getIn_block_position() - previous_block_usage];
        actual_position += loaded_block->getIn_block_position() - previous_block_usage;
        sync();
        version();
        if(loaded_block->getIn_block_position() == BLOCK_SIZE){
            delete loaded_block;
            if(loaded_block_index == blocks_hashes.size() - 1){
                loaded_block = new Block();
                blocks_hashes.push_back("blank");
                ++loaded_block_index;
            }
            else if(blocks_hashes[++loaded_block_index] != "blank"){
                load_block(blocks_hashes[loaded_block_index]);
            }
            else{
                loaded_block = new Block();
            }
        }
        previous_block_usage = loaded_block->getIn_block_position();
    }
}

void File::load_block(const std::string &hash)
{
    std::streamsize offset = (*vcm_meta)[META_BLOCKS_KEY][hash]["offset"];
    char *content = new char[offset];
    std::ifstream system_blocks(VCM_BLOCKS_FILENAME, std::ios_base::binary);
    unsigned long position = (*vcm_meta)[META_BLOCKS_KEY][hash]["pos"];
    system_blocks.seekg(position);
    system_blocks.read(content, offset);
    loaded_block = new Block(content, offset);
    system_blocks.close();
}
