project('opengl c++ skeleton', 'cpp',
  version : '0.1.0',
  license : 'MIT')

compiler = meson.get_compiler('cpp')
cppabi_lib = compiler.find_library('c++abi', dirs : ['/usr/lib/', '/usr/local/lib'])
cpp_lib = compiler.find_library('c++', dirs : ['/usr/lib/', '/usr/local/lib'])

lld = find_program('lld')

glfw_dep = dependency('glfw3')
glew_dep = dependency('glew')
gl_dep = dependency('gl')

cpp_arguments = ['-Wall', '-pedantic', '-std=c++1z']
link_arguments = []
if cppabi_lib.found()
  link_arguments += ['-lc++abi']
  cpp_arguments += ['-stdlib=libc++']
endif
if cpp_lib.found()
  link_arguments += ['-lc++']
endif
if lld.found()
  link_arguments += ['-fuse-ld=lld']
endif


executable('main', 
  sources : 
    [
      'main.cc', 
      'shader.cc'
    ],
  cpp_args : cpp_arguments,
  link_args : link_arguments,
  dependencies :
    [
      glfw_dep, 
      glew_dep, 
      gl_dep
    ]
  )