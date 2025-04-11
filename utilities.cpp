#include "utilities.h"
#include <fstream>
#include <openssl/sha.h>
std::string get_time_stamp() //este lo hizo chati
{
    std::time_t now = std::time(nullptr);
    std::tm* tm_info = std::localtime(&now);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

    return std::string(buffer);
}

void save_json(const json &data, const std::string &filename)
{
    std::ofstream file(filename);
    if (file.is_open()) {
        file << data.dump(4);  // dump(4) para indentado limdo
        file.close();
    }
}

std::string sha256(const char* data, size_t length) {
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256(reinterpret_cast<const unsigned char*>(data), length, hash);

    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);

    return oss.str();
}
