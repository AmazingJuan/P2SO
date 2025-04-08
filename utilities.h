#include <string>
#include <ctime>
#include "json.hpp"

using json = nlohmann::json;

std::string get_time_stamp();
std::string sha256(const std::string &str);
void save_json(const json& data, const std::string& filename);
