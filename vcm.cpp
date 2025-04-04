#include "vcm.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
VCM::VCM() {
    if (!fs::exists(VCM_BLOCKS_FILENAME) || !fs::is_regular_file(VCM_BLOCKS_FILENAME)){
        std::ofstream archivo(VCM_BLOCKS_FILENAME);
        archivo.close();
    }

    if (!fs::exists(VCM_META_FILENAME) || !fs::is_regular_file(VCM_META_FILENAME)){
        std::ofstream archivo(VCM_META_FILENAME);
        json j;
        j["blocks"] = "";
        archivo << j.dump(4);  // dump(4) para indentado limdo
        archivo.close();

    }

}

File VCM::create(const std::string filename)
{
    opened_files[filename] = new File(filename, true);
    return *opened_files[filename];
}
