#version 330

in vec3 v_position;
in vec3 v_color;
out vec3 f_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

  gl_Position = projection * view * model * vec4(v_position, 1.f);
  gl_PointSize = 3.f;
  f_color = v_color;

}