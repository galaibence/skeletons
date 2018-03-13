cl_dep = dependency('OpenCL')

include_directories = include_directories('include') 

cpp_arguments = [
    '-Wall', 
    '-pedantic', 
    '-std=c++17',
]

link_arguments = []

sources = [
  'src/main.cc', 
]

dependencies = [
  cl_dep,
]

executable('opencl_main', 
  sources : sources,
  include_directories : include_directories,
  cpp_args : cpp_arguments,
  link_args : link_arguments,
  dependencies : dependencies,
)