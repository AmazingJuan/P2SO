#include "utilities.h"
#include <fstream>
std::string get_time_stamp() //este lo hizo chati
{
    std::time_t now = std::time(nullptr);
    std::tm* tm_info = std::localtime(&now);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

    return std::string(buffer);
}


std::string md5(const std::string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH];  // 16 bytes (128 bits)
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), digest);

    std::ostringstream oss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }

    return oss.str();
}

bool hash_exists(const json& datos, const std::string& objetivo) {
    for (const auto& bloque : datos["blocks"]) {
        if (bloque.contains("hash") && bloque["hash"] == objetivo) {
            return true;
        }
    }
    return false;
}

void save_json(const json &data, const std::string &filename)
{
    std::ofstream file(filename);
    if (file.is_open()) {
        file << data.dump(4);  // dump(4) para indentado limdo
        file.close();
    }
}
