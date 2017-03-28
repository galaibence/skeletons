#include "shader.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "gl_headers.h"

void Shader::link() {
  program_ = glCreateProgram();
  for (auto shader : stages_) glAttachShader(program_, shader);
  glLinkProgram(program_);

  int success;
  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (!success) {
    int info_log_length;
    glGetShaderiv(program_, GL_INFO_LOG_LENGTH, &info_log_length);
    std::vector<char> info_log(info_log_length);
    glGetShaderInfoLog(program_, info_log_length, nullptr, info_log.data());

    std::cout << "SHADER LINKING FAILED:" << std::endl
              << info_log.data() << std::endl;
  }

  for (auto shader : stages_) {
    glDeleteShader(shader);
  }
}

void Shader::compile(uint32_t stage, std::string path) {
  std::string shader_stage;
  switch (stage) {
    case GL_VERTEX_SHADER:
      shader_stage = "VERTEX SHADER";
      break;
    case GL_GEOMETRY_SHADER:
      shader_stage = "GEOMETRY SHADER";
      break;
    case GL_TESS_CONTROL_SHADER:
      shader_stage = "TESSELLATION CONTROL SHADER";
      break;
    case GL_TESS_EVALUATION_SHADER:
      shader_stage = "TESSELLATION EVALUATION SHADER";
      break;
    case GL_FRAGMENT_SHADER:
      shader_stage = "FRAGMENT SHADER";
      break;
    case GL_COMPUTE_SHADER:
      shader_stage = "COMPUTE SHADER";
      break;
    default:
      std::cout << "Invalid shader stage" << std::endl;
      exit(-1);
  }

  std::ifstream shader_file;
  shader_file.open(path.c_str());
  if (!shader_file.is_open()) {
    std::cout << "Cannot load shader at " << path << std::endl;
    exit(-1);
  }

  std::stringstream shader_stream;
  shader_stream << shader_file.rdbuf();
  shader_file.close();
  std::string shader_code = shader_stream.str();
  const char* shader_code_cstr = shader_code.c_str();

  uint32_t shader = glCreateShader(stage);
  glShaderSource(shader, 1, &shader_code_cstr, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    int info_log_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
    std::vector<char> info_log(info_log_length);
    glGetShaderInfoLog(shader, info_log_length, nullptr, info_log.data());

    std::cout << shader_stage << " COMPILATION FAILED:" << std::endl
              << info_log.data() << std::endl;
  }

  stages_.push_back(shader);
}

uint32_t Shader::get() { return program_; }

void Shader::use() { glUseProgram(this->program_); }