#include "vcm.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "utilities.h"
namespace fs = std::filesystem;
VCM::VCM() {
    if (!fs::exists(VCM_BLOCKS_FILENAME) || !fs::is_regular_file(VCM_BLOCKS_FILENAME)){
        std::ofstream archivo(VCM_BLOCKS_FILENAME);
        archivo.close();
    }

    if (!fs::exists(VCM_META_FILENAME) || !fs::is_regular_file(VCM_META_FILENAME)){
        metadata = new json;
        (*metadata)[META_FILES_KEY] = json::array();
        (*metadata)[META_BLOCKS_KEY] = {};
        save_json(*metadata, VCM_META_FILENAME);
    }
    else{
        std::ifstream archivo(VCM_META_FILENAME);
        metadata = new json;
        *metadata = json::parse(archivo);
        archivo.close();
    }
}

File VCM::open(const std::string &filename)
{

    bool isFound = std::find((*metadata)[META_FILES_KEY].begin(), (*metadata)[META_FILES_KEY].end(), filename) != (*metadata)[META_FILES_KEY].end();
    if(isFound){
        std::ifstream meta_file(filename + FILE_METADATA_EXTENSION);
        json file_meta = json::parse(meta_file);
        if(file_meta[FILE_LATEST_CHECK_KEY] || !fs::exists(filename)){
            std::ofstream phys_file(filename, std::ios_base::binary);
            std::ifstream blocks_file(VCM_BLOCKS_FILENAME, std::ios_base::binary);
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

    }
    return create("");
}

File VCM::create(const std::string &filename)
{
    (*metadata)[META_FILES_KEY].push_back(filename);
    save_json(*metadata, VCM_META_FILENAME);
    opened_files[filename] = new File(filename, metadata, true);
    return *opened_files[filename];
}



