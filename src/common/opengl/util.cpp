#include "common/opengl/util.h"

#include <fstream>

std::vector<uint8_t> opengl::ReadFileBinary(std::string_view file_path) {
  std::vector<uint8_t> data;
  std::ifstream file_in(std::string(file_path),
                        std::ios::binary | std::ios::ate);
  if (!file_in.is_open())
    LOG_ERROR("Cannot open file {}.", file_path);
  size_t size = file_in.tellg();
  data.resize(size);
  file_in.seekg(std::ifstream::beg);
  file_in.read(reinterpret_cast<char *>(data.data()), static_cast<long>(size));
  file_in.close();
  // LOG_INFO("File {} size {}.", file_path, size);
  return data;
}
