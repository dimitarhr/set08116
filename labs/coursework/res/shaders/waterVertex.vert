#version 440

// Model transformation matrix
uniform mat4 M;
// Transformation matrix
uniform mat4 MVP;
// Normal matrix
uniform mat3 N;
uniform vec2 UV_SCROLL;
uniform vec2 UV_SCROLL_TWO;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 2) in vec3 normal;
// Incoming binormal
layout(location = 3) in vec3 binormal;
// Incoming tangent
layout(location = 4) in vec3 tangent;
// Incoming texture coordinate
layout(location = 10) in vec2 tex_coord_in;

// Outgoing position
layout (location = 0) out vec3 vertex_position;
// Outgoing transformed normal
layout (location = 1) out vec3 transformed_normal;
// Outgoing tangent
layout(location = 3) out vec3 tangent_out;
// Outgoing binormal
layout(location = 4) out vec3 binormal_out;
// Outgoing texture coordinate
layout(location = 5) out vec2 tex_coord_out;
layout(location = 6) out vec2 tex_coord_out_Two;

void main()
{
  // Calculate screen position
  float new_y = position.y + sin(UV_SCROLL.y);
  gl_Position = MVP * vec4(position.x,new_y,position.z, 1.0);
  // Output other values to fragment shader
  vertex_position = (M*vec4(position,1.0f)).xyz;
  transformed_normal = N * normal;
  // Transform tangent
  tangent_out = N * tangent;
  // Transform binormal
  binormal_out = N * binormal;  
  tex_coord_out = tex_coord_in + UV_SCROLL-2.0;
  tex_coord_out_Two = tex_coord_in + UV_SCROLL_TWO;
}