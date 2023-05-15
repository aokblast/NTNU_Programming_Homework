#include <algorithm>
#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <array>
#include <string>

using namespace std;
using data_t = std::vector<unsigned char>;

constexpr size_t block_size = 1024;

static data_t
sha256_calc(const std::vector<unsigned char> &data) {
  data_t result(SHA256_DIGEST_LENGTH);
  SHA256(data.data(), data.size(), result.data());
  return result;
}

int main(int argc, char *argv[]) {

  if(argc != 2) {
    std::cerr << "Argument Not Right\n";
    exit(0);
  }

  std::ifstream file_stream(argv[1], ios::binary);

  data_t result;

  std::vector<data_t> blocks;
  std::vector<char> tmp(block_size);

  while(file_stream.good()) {
    data_t block;
    file_stream.read(tmp.data(), block_size);
    int sz = file_stream.gcount();
    std::for_each(tmp.begin(), tmp.begin() + sz, [&block](char c){block.push_back(c);});
    blocks.emplace_back(std::move(block));
  }

  for(int i = blocks.size() - 1; i >= 0; --i){
    data_t cur;
    cur.insert(cur.end(), blocks[i].begin(), blocks[i].end());
    cur.insert(cur.end(), result.begin(), result.end());
    result = sha256_calc(cur);
  }


  for(int i = 0; i < result.size(); ++i) {
    cout << hex << setw(2) << setfill('0') << static_cast<int>(result[i]);
  }

  cout << endl;
}
