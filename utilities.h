#include <string>
#include <ctime>
#include <openssl/md5.h>
#include "json.hpp"
using json = nlohmann::json;

std::string get_time_stamp();
std::string md5(const std::string& input);
bool hash_exists(const json& datos, const std::string& objetivo);
void save_json(const json& data, const std::string& filename);
