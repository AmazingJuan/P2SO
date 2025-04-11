#include "vcm.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "utilities.h"

VCM::VCM() {
    if (!fs::exists(get_user_home_path() / VCM_BLOCKS_FILENAME) || !fs::is_regular_file(get_user_home_path() / VCM_BLOCKS_FILENAME)){
        std::ofstream archivo(get_user_home_path() / VCM_BLOCKS_FILENAME);
        archivo.close();
    }

    if (!fs::exists(get_user_home_path() / VCM_META_FILENAME) || !fs::is_regular_file(get_user_home_path() / VCM_META_FILENAME)){
        metadata = new json;
        (*metadata)[META_FILES_KEY] = json::array();
        (*metadata)[META_BLOCKS_KEY] = {};
    }
    else{
        std::ifstream archivo(get_user_home_path() / VCM_META_FILENAME);
        metadata = new json;
        *metadata = json::parse(archivo);
        archivo.close();
    }
}

VCM::~VCM()
{
    for (auto& pair : opened_files) {
        delete pair.second;
    }
    opened_files.clear();
    save_json(*metadata, (get_user_home_path() / VCM_META_FILENAME).string());
}

File &VCM::open(const std::string &filename)
{
    bool isFound = std::find((*metadata)[META_FILES_KEY].begin(), (*metadata)[META_FILES_KEY].end(), filename) != (*metadata)[META_FILES_KEY].end();
    if(isFound && fs::exists(filename + FILE_METADATA_EXTENSION)){
        std::ifstream meta_file(filename + FILE_METADATA_EXTENSION);
        json file_meta = json::parse(meta_file);
        if(!fs::exists(filename)){
            std::ofstream phys_file(filename, std::ios_base::binary);
            std::ifstream blocks_file(get_user_home_path() / VCM_BLOCKS_FILENAME, std::ios_base::binary);
            for(std::string hash : file_meta[FILE_VERSIONS_KEY][0]["blocks"]){
                unsigned long position = (*metadata)[META_BLOCKS_KEY][hash]["pos"];
                unsigned long offset = (*metadata)[META_BLOCKS_KEY][hash]["offset"];
                char *block = new char[offset];
                blocks_file.seekg(position);
                blocks_file.read(block, offset);
                phys_file.write(block, offset);
                delete [] block;
            }
            phys_file.close();
            blocks_file.close();
        }
        opened_files[filename] = new File(filename, metadata, false);
        return *opened_files[filename];
    }
    else{
        return create(filename);
    }
}

File &VCM::create(const std::string &filename)
{
    bool handle_flag = false;
    if(fs::exists(filename) && !fs::exists(filename + FILE_METADATA_EXTENSION)){
        handle_flag = true;
    }
    (*metadata)[META_FILES_KEY].push_back(filename);
    opened_files[filename] = new File(filename, metadata, true);
    if(handle_flag){
        opened_files[filename]->handle_strange();
    }
    return *opened_files[filename];
}

unsigned long VCM::memory_usage()
{
    size_t total = sizeof(VCM);

    // Tamaño del map + sus claves
    total += opened_files.size() * (sizeof(std::string) + sizeof(File*));
    for (const auto& pair : opened_files) {
        total += pair.first.capacity() * sizeof(char);
        if (pair.second) {
            total += pair.second->memory_usage(); // delegamos a File
        }
    }

    // Estimación del JSON
    if (metadata) {
        std::string dump = metadata->dump(); // convierte a string temporal
        total += dump.capacity() * sizeof(char);
    }

    return total;
}



