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

char* fill_buffer_return_remainder(char* buffer, int max_size, const char* input, int start_index, int& next_index_out) {
    int input_len = std::strlen(input);

    // Calcular cuántos bytes quedan desde start_index
    int remaining = input_len - start_index;
    int to_copy = (remaining > max_size) ? max_size : remaining;

    // Copiar al buffer
    for (int i = 0; i < to_copy; ++i) {
        buffer[i] = input[start_index + i];
    }

    // Rellenar el resto con ceros si quedó espacio (opcional)
    for (int i = to_copy; i < max_size; ++i) {
        buffer[i] = '\0';
    }

    // Calcular el nuevo índice
    next_index_out = start_index + to_copy;

    // Crear el string restante
    int leftover_len = input_len - next_index_out;
    if (leftover_len <= 0) {
        // Nada queda
        char* empty = (char*)malloc(1);
        empty[0] = '\0';
        return empty;
    }

    // Reservar memoria y copiar el resto
    char* remainder = (char*)malloc(leftover_len + 1);
    for (int i = 0; i < leftover_len; ++i) {
        remainder[i] = input[next_index_out + i];
    }
    remainder[leftover_len] = '\0';

    return remainder;
}
