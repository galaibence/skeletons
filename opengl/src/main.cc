#include <cassert>
#include <iostream>
#include <random>

#include "gl_headers.h"
#include "shader.h"

int width_;
int height_;

uint32_t VAO;
uint32_t position_VBO;
uint32_t color_VBO;

std::vector<float> position;
std::vector<float> color;

GLFWwindow* window_;

Shader shader_;

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void parseInput(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "Not enough input arguments!" << std::endl
              << "Proper usage: " << std::endl
              << "  ./main width height" << std::endl;
    exit(-1);
  }

  width_ = atoi(argv[1]);
  height_ = atoi(argv[2]);
}

void initGLFW() {
  assert(glfwInit());

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  window_ = glfwCreateWindow(width_, height_, "OpenGL Skeleton Window", nullptr,
                             nullptr);
  assert(window_ != nullptr);
  glfwMakeContextCurrent(window_);

  glfwSetKeyCallback(window_, keyCallback);
  // register other callback functions
}

void initGLEW() {
  glewExperimental = GL_TRUE;
  assert(GLEW_OK == glewInit());
}

void initDraw() {
  glViewport(0, 0, width_, height_);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PROGRAM_POINT_SIZE);

  shader_.compile(GL_VERTEX_SHADER, "../../shaders/shader.vert");
  shader_.compile(GL_FRAGMENT_SHADER, "../../shaders/shader.frag");
  shader_.link();
}

void init(int argc, char* argv[]) {
  parseInput(argc, argv);

  initGLFW();
  initGLEW();
  initDraw();

  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(0.f, 1.f);

  position.resize(1000 * 3);
  color.resize(1000 * 3);
  for (int i = 0; i < 1000; i++) {
    position.push_back(distribution(generator) - 0.5f);
    position.push_back(distribution(generator) - 0.5f);
    position.push_back(distribution(generator) - 0.5f);

    color.push_back(distribution(generator));
    color.push_back(distribution(generator));
    color.push_back(distribution(generator));
  }

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &position_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, position_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * position.size(),
               position.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &color_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * color.size(), color.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void update() {
  // do some logic here
}

void render() {
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_.use();

  glm::mat4 model = glm::mat4();
  int model_location = glGetUniformLocation(shader_.get(), "model");
  glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

  glm::mat4 view =
      glm::lookAt(glm::vec3{0.f, 0.f, 1.f}, glm::vec3{0.f, 0.f, 0.f},
                  glm::vec3{0.f, 1.f, 0.f});
  int view_location = glGetUniformLocation(shader_.get(), "view");
  glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

  glm::mat4 projection =
      glm::perspective(45.f, (float)width_ / (float)height_, 0.1f, 1000.f);
  int projection_location = glGetUniformLocation(shader_.get(), "projection");
  glUniformMatrix4fv(projection_location, 1, GL_FALSE,
                     glm::value_ptr(projection));

  glBindVertexArray(VAO);
  glDrawArrays(GL_POINTS, 0, position.size() / 3);
  glBindVertexArray(0);
}

void loop() {
  while (!glfwWindowShouldClose(window_)) {
    glfwPollEvents();

    update();
    render();

    glfwSwapBuffers(window_);
  }
}

int main(int argc, char* argv[]) {
  init(argc, argv);
  loop();

  return 0;
}