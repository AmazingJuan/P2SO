/*
 * SHA.h
 *
 * This header file defines classes for implementing various SHA (Secure Hash
 * Algorithm) functions in C++. It includes:
 *
 * - SHABase: A base class containing common methods and utilities used by SHA
 * algorithms.
 * - SHA256: Implements the SHA-256 hashing function.
 * - SHA224: Implements the SHA-224 hashing function, which is a truncated
 * version of SHA-256.
 * - SHA512: Implements the SHA-512 hashing function.
 * - SHA384: Implements the SHA-384 hashing function, which is a truncated
 * version of SHA-512.
 * - SHA512_224: Implements the SHA-512/224 hashing function.
 * - SHA512_256: Implements the SHA-512/256 hashing function.
 *
 * The file provides functionality to compute cryptographic hash values for
 * given input data. Each class supports hashing with optional initial hash
 * values and provides methods to format and convert the output into a
 * hexadecimal string representation.
 *
 * This file includes necessary constants, utility functions, and detailed
 * implementations of the SHA algorithms as specified by the NIST standards.
 */

#ifndef SHA_H_
#define SHA_H_

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>
#include <sstream>

namespace sha {

static constexpr uint64_t CONST_SHA512_H[8] = {
    0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b,
    0xa54ff53a5f1d36f1, 0x510e527fade682d1, 0x9b05688c2b3e6c1f,
    0x1f83d9abfb41bd6b, 0x5be0cd19137e2179};
static constexpr uint64_t CONST_SHA512_224_H[8] = {
    0x8c3d37c819544da2, 0x73e1996689dcd4d6, 0x1dfab7ae32ff9c82,
    0x679dd514582f9fcf, 0x0f6d2b697bd44da8, 0x77e36f7304c48942,
    0x3f9d85a86a1d36c8, 0x1112e6ad91d692a1};
static constexpr uint64_t CONST_SHA512_256_H[8] = {
    0x22312194fc2bf72c, 0x9f555fa3c84c64c2, 0x2393b86b6f53b151,
    0x963877195940eabd, 0x96283ee2a88effe3, 0xbe5e1e2553863992,
    0x2b0199fc2c85b8aa, 0x0eb72ddc81c52ca2};
static constexpr uint64_t CONST_SHA384_H[8] = {
    0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17,
    0x152fecd8f70e5939, 0x67332667ffc00b31, 0x8eb44a8768581511,
    0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4};
static constexpr uint32_t CONST_SHA256_H[8] = {
    0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
    0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};
static constexpr uint32_t CONST_SHA224_H[8] = {
    0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
    0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4};

static constexpr uint32_t SHA256_K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

static constexpr uint64_t SHA512_K[] = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
    0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
    0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
    0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
    0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
    0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
    0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
    0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
    0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
    0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
    0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
    0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
    0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
    0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
    0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
    0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
    0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
    0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
    0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
    0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
    0x5fcb6fab3ad6faec, 0x6c44198c4a475817};

class SHABase {
 protected:
  // Converts a number to a bit string representation.
  template <typename Type>
  std::string to_bit_string(Type number) const {
    std::string bit_string;
    bit_string.reserve(sizeof(number));
    for (int i = 7; i >= 0; i--) {
      bit_string.push_back(number >> i * 8);
    }
    return bit_string;
  }

  // Converts a character array to an integral type representation.
  template <typename Type>
  Type to_integral(const char* data) const {
    Type value = 0;
    for (int i = 0; i < sizeof(Type); i++) {
      value |= (Type)(uint8_t)data[sizeof(Type) - 1 - i] << i * 8;
    }
    return value;
  }

  // Converts a preprocessed string into a vector of integral types.
  template <typename Type>
  std::vector<Type> to_integral_vector(
      const std::string& preprocessed_data) const {
    std::vector<Type> data_in_integral(preprocessed_data.size() / sizeof(Type));
    int k = 0;
    for (int i = 0; i < data_in_integral.size(); i++) {
      data_in_integral[i] = to_integral<Type>(preprocessed_data.c_str() + k);
      k += sizeof(Type);
    }
    return data_in_integral;
  }

  // Converts a string to its hexadecimal representation.
  std::string to_hex(const std::string& str) const {
    std::ostringstream oss;
    for (const char& ch : str) {
      unsigned int x = (unsigned char)ch;
      oss << std::hex << std::setw(2) << std::setfill('0') << x;
    }
    return oss.str();
  }

  // Converts an array of hash values to a string representation.
  template <typename Type>
  std::string to_string(const Type* hash_digest) const {
    std::string str_repr;
    str_repr.reserve(sizeof(Type) * 8);
    for (int i = 0; i < 8; i++) {
      for (int j = sizeof(Type) - 1; j >= 0; j--) {
        str_repr.push_back(hash_digest[i] >> j * 8);
      }
    }
    return str_repr;
  }

  // Computes the 'ch' function for hash computations.
  template <typename Type>
  constexpr Type ch(Type x, Type y, Type z) const {
    return (x & y) ^ ((~x) & z);
  }

  // Computes the 'maj' function for hash computations.
  template <typename Type>
  constexpr Type maj(Type x, Type y, Type z) const {
    static_assert(std::is_integral<Type>::value,
                  "Type must be an integral type");
    return (x & y) ^ (x & z) ^ (y & z);
  }

  // Performs a right bitwise rotation.
  template <typename Type>
  constexpr Type RotR(Type a, short n) const {
    return (a >> n) | (a << (sizeof(Type) * 8 - n));
  }

  // Performs a right arithmetic shift.
  template <typename Type>
  constexpr Type ShR(Type a, short n) const {
    return a >> n;
  }
};

class SHA256 : public SHABase {
 private:
  // Pads and formats input data to a multiple of 512 bits with length encoding.
  std::string prepare_input(const char* data) const {
    uint64_t len = strlen(data);  // Length of input data
    uint64_t total_len =
        len + 8 + 1;  // Length including size and padding for 1 byte
    size_t pad_len = ((total_len / 64) + 1) * 64 - total_len;
    std::string preprocessed_data;
    preprocessed_data.reserve(total_len + pad_len);
    preprocessed_data.append(data);
    preprocessed_data.push_back(0b10000000);

    if (pad_len != 0) {
      std::string bytes(pad_len, '\0');
      preprocessed_data.append(bytes);
    }

    preprocessed_data.append(to_bit_string<uint64_t>(len * 8));
    return preprocessed_data;
  }

  // Processes a 512-bit block and updates the SHA-256 hash values.
  void process_block(const std::vector<uint32_t>& data_in_uint32_t,
                     size_t block_index, uint32_t* hash_values) const {
    std::vector<uint32_t> words(64);
    std::copy(data_in_uint32_t.begin() + (16 * block_index),
              data_in_uint32_t.begin() + (16 * block_index) + 16,
              words.begin());

    for (int i = 16; i < 64; i++) {
      words[i] = small_sigma_1(words[i - 2]) + words[i - 7] +
                 small_sigma_0(words[i - 15]) + words[i - 16];
    }

    uint32_t a = hash_values[0];
    uint32_t b = hash_values[1];
    uint32_t c = hash_values[2];
    uint32_t d = hash_values[3];
    uint32_t e = hash_values[4];
    uint32_t f = hash_values[5];
    uint32_t g = hash_values[6];
    uint32_t h = hash_values[7];

    for (int i = 0; i < 64; i++) {
      uint32_t T1 = h + big_sigma_1(e) + ch(e, f, g) + SHA256_K[i] + words[i];
      uint32_t T2 = big_sigma_0(a) + maj(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + T1;
      d = c;
      c = b;
      b = a;
      a = T1 + T2;
    }

    hash_values[0] += a;
    hash_values[1] += b;
    hash_values[2] += c;
    hash_values[3] += d;
    hash_values[4] += e;
    hash_values[5] += f;
    hash_values[6] += g;
    hash_values[7] += h;
  }

  // Applies the big_sigma_0 function as defined by SHA-256 to input x
  constexpr uint32_t big_sigma_0(uint32_t x) const {
    return RotR<uint32_t>(x, 2) ^ RotR<uint32_t>(x, 13) ^ RotR<uint32_t>(x, 22);
  }

  // Applies the big_sigma_1 function as defined by SHA-256 to input x
  constexpr uint32_t big_sigma_1(uint32_t x) const {
    return RotR<uint32_t>(x, 6) ^ RotR<uint32_t>(x, 11) ^ RotR<uint32_t>(x, 25);
  }

  // Applies the small_sigma_0 function as defined by SHA-256 to input x
  constexpr uint32_t small_sigma_0(uint32_t x) const {
    return RotR<uint32_t>(x, 7) ^ RotR<uint32_t>(x, 18) ^ ShR<uint32_t>(x, 3);
  }

  // Applies the small_sigma_1 function as defined by SHA-256 to input x
  constexpr uint32_t small_sigma_1(uint32_t x) const {
    return RotR<uint32_t>(x, 17) ^ RotR<uint32_t>(x, 19) ^ ShR<uint32_t>(x, 10);
  }

 protected:
  // Computes the SHA-256 hash of the input data using the given initial hash
  // values and returns the result as a hexadecimal string.
  std::string __hash(const char* data, const uint32_t* init_hash) const {
    uint32_t hash_vals[8];
    std::memcpy(hash_vals, init_hash, 32);
    std::string preprocessed_data = prepare_input(data);
    std::vector<uint32_t> data_in_uint32_t =
        to_integral_vector<uint32_t>(preprocessed_data);
    size_t N = preprocessed_data.size() / 64;
    for (int block = 0; block < N; block++) {
      process_block(data_in_uint32_t, block, hash_vals);
    }
    std::string hashed_value = to_string(hash_vals);
    return to_hex(hashed_value);
  }

 public:
  // Computes the SHA-256 hash of the input data using optional initial hash
  // values.
  std::string hash(const char* data) const {
    return __hash(data, CONST_SHA256_H);
  }
};

class SHA224 : public SHA256 {
 public:
  // Computes a SHA-224 hash from input data.
  std::string hash(const char* data) const {
    std::string hashed_digest = __hash(data, CONST_SHA224_H);
    hashed_digest.resize(56);
    return hashed_digest;
  }
};

class SHA512 : public SHABase {
 private:
  std::string prepare_input(const char* data) const {
    size_t len = strlen(data);
    uint64_t total_len = len + 8 + 1;
    size_t pad_len = ((total_len / 128) + 1) * 128 - total_len;
    std::string preprocessed_data;
    preprocessed_data.reserve(total_len + pad_len);
    preprocessed_data.append(data);
    preprocessed_data.push_back(0b10000000);

    if (pad_len != 0) {
      std::string bytes(pad_len, '\0');
      preprocessed_data.append(bytes);
    }

    preprocessed_data.append(to_bit_string<uint64_t>(len * 8));
    return preprocessed_data;
  }

  void process_block(const std::vector<uint64_t>& data_in_uint64_t,
                     size_t block_index, uint64_t* hash_values) const {
    std::vector<uint64_t> words(80);
    std::copy(data_in_uint64_t.begin() + (16 * block_index),
              data_in_uint64_t.begin() + (16 * block_index) + 16,
              words.begin());

    for (int i = 16; i < 80; i++) {
      words[i] = small_sigma_1(words[i - 2]) + words[i - 7] +
                 small_sigma_0(words[i - 15]) + words[i - 16];
    }

    uint64_t a = hash_values[0];
    uint64_t b = hash_values[1];
    uint64_t c = hash_values[2];
    uint64_t d = hash_values[3];
    uint64_t e = hash_values[4];
    uint64_t f = hash_values[5];
    uint64_t g = hash_values[6];
    uint64_t h = hash_values[7];

    for (int i = 0; i < 80; i++) {
      uint64_t T1 =
          h + big_sigma_1(e) + ch<uint64_t>(e, f, g) + SHA512_K[i] + words[i];
      uint64_t T2 = big_sigma_0(a) + maj<uint64_t>(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + T1;
      d = c;
      c = b;
      b = a;
      a = T1 + T2;
    }

    hash_values[0] += a;
    hash_values[1] += b;
    hash_values[2] += c;
    hash_values[3] += d;
    hash_values[4] += e;
    hash_values[5] += f;
    hash_values[6] += g;
    hash_values[7] += h;
  }

  // Applies the big sigma_0 transformation for SHA-512.
  constexpr uint64_t big_sigma_0(uint64_t x) const {
    return RotR<uint64_t>(x, 28) ^ RotR<uint64_t>(x, 34) ^
           RotR<uint64_t>(x, 39);
  }

  // Applies the big sigma_1 transformation for SHA-512.
  constexpr uint64_t big_sigma_1(uint64_t x) const {
    return RotR<uint64_t>(x, 14) ^ RotR<uint64_t>(x, 18) ^
           RotR<uint64_t>(x, 41);
  }

  // Applies the small sigma_0 transformation for SHA-512.
  constexpr uint64_t small_sigma_0(uint64_t x) const {
    return RotR<uint64_t>(x, 1) ^ RotR<uint64_t>(x, 8) ^ ShR<uint64_t>(x, 7);
  }

  // Applies the small sigma_1 transformation for SHA-512.
  constexpr uint64_t small_sigma_1(uint64_t x) const {
    return RotR<uint64_t>(x, 19) ^ RotR<uint64_t>(x, 61) ^ ShR<uint64_t>(x, 6);
  }

 protected:
  // Hashes the input data using an initial hash value and returns the result as
  // a hexadecimal string.
  std::string __hash(const char* data, const uint64_t* init_hash) const {
    uint64_t hash_vals[8];
    std::memcpy(hash_vals, init_hash, 64);

    std::string preprocessed_data = prepare_input(data);
    std::vector<uint64_t> data_in_uint64_t =
        to_integral_vector<uint64_t>(preprocessed_data);

    size_t N = preprocessed_data.size() / 128;
    for (int block = 0; block < N; block++) {
      process_block(data_in_uint64_t, block, hash_vals);
    }
    std::string hashed_value = to_string(hash_vals);
    return to_hex(hashed_value);
  }

 public:
  // Computes a SHA-512 hash from input data.
  std::string hash(const char* data) const {
    return __hash(data, CONST_SHA512_H);
  }
};

class SHA384 : public SHA512 {
 public:
  // Computes a SHA-384 hash from input data.
  std::string hash(const char* data) const {
    std::string hash_digest = __hash(data, CONST_SHA384_H);
    hash_digest.resize(96);
    return hash_digest;
  }
};

class SHA512_224 : public SHA512 {
 public:
  // Computes a SHA-512/224 hash from input data and returns it as a
  // 56-character string.
  std::string hash(const char* data) const {
    std::string hash_digest = __hash(data, CONST_SHA512_224_H);
    hash_digest.resize(56);
    return hash_digest;
  }
};

class SHA512_256 : public SHA512 {
 public:
  // Computes a SHA-512/256 hash from input data.
  std::string hash(const char* data) const {
    std::string hash_digest = __hash(data, CONST_SHA512_256_H);
    hash_digest.resize(64);
    return hash_digest;
  }
};
}  // namespace sha

#endif  // SHA_H_
