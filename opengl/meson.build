
glfw_dep = dependency('glfw3')
glew_dep = dependency('glew')
gl_dep = dependency('gl')

include_directories = include_directories('include') 

cpp_arguments = ['-Wall', '-pedantic', '-std=c++17']

link_arguments = []

sources = [
  'src/main.cc', 
  'src/shader.cc'
]

dependencies = [
  glfw_dep,
  glew_dep,
  gl_dep
]

executable('opengl_main', 
  sources : sources,
  include_directories : include_directories,
  cpp_args : cpp_arguments,
  link_args : link_arguments,
  dependencies : dependencies
)