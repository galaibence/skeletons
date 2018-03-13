#ifndef VISUALIZATION_SHADER_H
#define VISUALIZATION_SHADER_H

#include <cstdint>
#include <string>
#include <vector>

class Shader {
 private:
  uint32_t program_;
  std::vector<uint32_t> stages_;

 public:
  Shader() = default;

  void link();
  void compile(uint32_t stage, std::string path);

  uint32_t get();
  void use();
};

#endif