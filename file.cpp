#include "file.h"
#include <fstream>
#include <iostream>
#include "vcm.h"
#include "utilities.h"
#include <format>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;
File::File(const std::string &filename, json *system_meta, bool recently_created)
{
    this->filename = fs::absolute(filename).string();
    is_open = true;
    if (recently_created) {
        if (!fs::exists(filename)) {
            std::ofstream archivo(filename);
            archivo.close();
        }

        std::ofstream archivo2(filename + FILE_METADATA_EXTENSION);
        archivo2.close();

        json j;
        j[FILE_VERSIONS_KEY] = json::array();
        j[FILE_LATEST_CHECK_KEY] = true;
        j[FILE_LATEST_VERSION] = 0;

        metadata = j;
        loaded_block = new Block();
        unwritten_block = loaded_block;
        latest_version = 0;
        loaded_block_index = 0;
        blocks_hashes.push_back("hash");
    } else {
        std::ifstream meta_file(filename + FILE_METADATA_EXTENSION);
        json file_meta = json::parse(meta_file);
        meta_file.close();
        metadata = file_meta;

        std::ifstream vcm_blocks(get_user_home_path() / VCM_BLOCKS_FILENAME, std::ios_base::binary);
        for (const std::string& block_hash : file_meta[FILE_VERSIONS_KEY][0]["blocks"]) {
            blocks_hashes.push_back(block_hash);
        }
        if (!blocks_hashes.empty()) {
            std::string hash = blocks_hashes[0];
            unsigned long position = (*system_meta)[META_BLOCKS_KEY][hash]["pos"];
            std::streamsize offset = (*system_meta)[META_BLOCKS_KEY][hash]["offset"];

            std::vector<char> content(offset);
            vcm_blocks.seekg(position);
            vcm_blocks.read(content.data(), offset);

            loaded_block = new Block(content, offset);
            loaded_block_index = 0;
            unwritten_block = nullptr;
        } else {
            loaded_block = new Block();
            unwritten_block = loaded_block;
            blocks_hashes.push_back("hash");
        }
    }
    current_version = 0;
    actual_position = 0;
    buffer = std::vector<char>(BUFFER_SIZE, 0);
    buffer_usage = 0;
    this->vcm_meta = system_meta;
    latest_version = metadata[FILE_LATEST_VERSION];

    if (!metadata[FILE_LATEST_CHECK_KEY]) {
        change_version(latest_version);
    }
}

File::~File()
{
    if(!is_open){
        close();
    }
}

void File::write(const char* content, std::streamsize stream_size)
{
    if(is_open){
        unsigned long left_buffer_streamsize = stream_size;
        const char *left_buffer_content = fill_buffer(content, stream_size);
        left_buffer_streamsize -= buffer_usage;

        if(left_buffer_streamsize > 0 || buffer_usage == BUFFER_SIZE)
        {
            transfer_to_block(buffer.data(), buffer_usage);
            std::fill(buffer.begin(), buffer.end(), 0);
            buffer_usage = 0;
            if(left_buffer_streamsize > 0){
                write(left_buffer_content, left_buffer_streamsize);
            }
        }
    }
}

void File::read(char* content, std::streamsize stream_size)
{
    if(is_open){
        std::streamsize left_streamsize = stream_size;
        unsigned long contentPos = 0;
        while(left_streamsize != 0){
            content[contentPos] = loaded_block->getByte();
            if(loaded_block->getIn_block_position() == BLOCK_SIZE && loaded_block_index + 1 != blocks_hashes.size()){
                load_block(blocks_hashes[loaded_block_index + 1]);
                loaded_block_index++;
            }
            else if(loaded_block->getIn_block_position() == BLOCK_SIZE && loaded_block_index + 1 == blocks_hashes.size()){
                break;
            }
            left_streamsize--;
            contentPos++;
        }
    }
    move(0);
}

void File::sync()
{
    std::fstream archivo(filename, std::ios::in | std::ios::out | std::ios_base::binary);
    archivo.seekp(actual_position - loaded_block->getIn_block_position());
    archivo.write(loaded_block->getContent(), loaded_block->getBlock_usage());
    archivo.close();
    std::string current_block_hash = loaded_block->getHash();
    check_new_block(current_block_hash);
}

void File::version()
{
    latest_version++;
    current_version = latest_version;
    metadata[FILE_LATEST_VERSION] = latest_version;

    metadata[FILE_VERSIONS_KEY].insert(metadata[FILE_VERSIONS_KEY].begin(), json{{"id", latest_version}, {"blocks", blocks_hashes}, {"timestamp", get_time_stamp()}});

}

void File::check_new_block(const std::string &hash)
{
    if ((*vcm_meta)[META_BLOCKS_KEY].is_null() || !(*vcm_meta)[META_BLOCKS_KEY].contains(hash)){
        std::ofstream vcm_blocks(get_user_home_path() / VCM_BLOCKS_FILENAME, std::ios::app | std::ios_base::binary);
        vcm_blocks.seekp(0, std::ios::end);
        unsigned int pos = vcm_blocks.tellp();
        (*vcm_meta)[META_BLOCKS_KEY][hash] =  {{"offset", loaded_block->getBlock_usage()}, {"pos", pos}};
        vcm_blocks.write(loaded_block->getContent(), loaded_block->getBlock_usage());
        vcm_blocks.close();
    }
}

void File::next_block()
{
    delete loaded_block;
    if(loaded_block_index == blocks_hashes.size() - 1){
        loaded_block = new Block();
        blocks_hashes.push_back("blank");
        unwritten_block = loaded_block;
        ++loaded_block_index;
    }
    else{
        load_block(blocks_hashes[loaded_block_index]);
    }
}

void File::change_version(unsigned long id)
{
    if(is_open){
        if(id != current_version && id <= latest_version){
            current_version = id;
            auto blocks = metadata[FILE_VERSIONS_KEY][latest_version - id]["blocks"];
            std::ofstream aux_file(filename, std::ios_base::binary);
            std::ifstream blocks_file(get_user_home_path() / VCM_BLOCKS_FILENAME, std::ios_base::binary);
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
            }
            else if(!metadata[FILE_LATEST_CHECK_KEY]){
                metadata[FILE_LATEST_CHECK_KEY] = true;
            }
            blocks_file.close();
            aux_file.close();
        }
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
    if(is_open){
        unsigned long position_counter = 0;
        unsigned long counter = 0;
        for(std::string hash : blocks_hashes){
            std::streamsize offset;
            if(hash == "blank"){
                offset = unwritten_block->getBlock_usage();
            }
            else{
                offset = (*vcm_meta)[META_BLOCKS_KEY][hash]["offset"];
            }

            if(desired_position >= position_counter && desired_position < position_counter + offset){
                if(hash != loaded_block->getHash()){
                    if(loaded_block != unwritten_block){
                        delete loaded_block;
                    }
                    load_block(hash);
                }
                actual_position = desired_position;
                loaded_block->setIn_block_position((desired_position - position_counter)%offset);
                loaded_block_index = counter;
                std::fill(buffer.begin(), buffer.end(), 0);
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
    if(is_open){
        if(buffer_usage > 0){
            transfer_to_block(buffer.data(), buffer_usage);
        }
        save_json(metadata, filename + FILE_METADATA_EXTENSION);
        std::vector<std::string>().swap(blocks_hashes);
        std::vector<char>().swap(buffer);
        std::string().swap(filename);
        is_open = false;
    }
}

void File::handle_strange() {
    blocks_hashes.clear(); // Limpiamos todo para empezar desde cero

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return;
    }

    std::streamsize filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(BLOCK_SIZE);
    std::streamsize total_read = 0;

    while (total_read < filesize) {
        std::streamsize to_read = std::min(BLOCK_SIZE, int(filesize - total_read));

        file.read(buffer.data(), to_read);
        delete loaded_block;
        loaded_block = new Block(buffer, to_read);

        std::string hash = loaded_block->getHash();
        check_new_block(hash);
        blocks_hashes.push_back(hash);

        total_read += to_read;
    }
    file.close();
    version();
    move(0);
}

unsigned long File::memory_usage()
{
    unsigned long total = sizeof(File);
    total += filename.capacity() * sizeof(char);
    if (loaded_block) {
        total += sizeof(Block) + 4096 * sizeof(char);
    }
    if (unwritten_block) {
        total += sizeof(Block) + 4096 * sizeof(char);
    }
    total += blocks_hashes.capacity() * sizeof(std::string);
    for (const auto& hash : blocks_hashes) {
        total += hash.capacity() * sizeof(char);
    }
    total += buffer.capacity() * sizeof(char);
    std::string dump = metadata.dump();
    total += dump.capacity() * sizeof(char);

    return total;
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
        //version();

        if(loaded_block->getIn_block_position() == BLOCK_SIZE){
            next_block();
        }
        previous_block_usage = loaded_block->getIn_block_position();
    }
}

void File::load_block(const std::string &hash)
{
    if(hash == "blank"){
        loaded_block = unwritten_block;
    }
    else{
        std::streamsize offset = (*vcm_meta)[META_BLOCKS_KEY][hash]["offset"];
        std::vector<char> content(offset);

        std::ifstream system_blocks(get_user_home_path() / VCM_BLOCKS_FILENAME, std::ios_base::binary);
        unsigned long position = (*vcm_meta)[META_BLOCKS_KEY][hash]["pos"];
        system_blocks.seekg(position);
        system_blocks.read(content.data(), offset);

        loaded_block = new Block(content, offset);
        system_blocks.close();
    }
}

