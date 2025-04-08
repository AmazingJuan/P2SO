#include "utilities.h"
#include <fstream>
#include "sha.h"
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

std::string sha256(const std::string &str)
{
    sha::SHA256 sha256;
    return sha256.hash(str.c_str());
}
