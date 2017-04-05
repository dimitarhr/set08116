#version 440

// Model transformation matrix
uniform mat4 M;
// Transformation matrix
uniform mat4 MVP;
// Normal matrix
uniform vec2 UV_SCROLL;
uniform vec2 UV_SCROLL_TWO;
uniform vec3 cameraPostion;
uniform vec3 lightPosition;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming texture coordinate
layout(location = 10) in vec2 tex_coord_in;


// Outgoing texture coordinate
layout(location = 2) out vec2 tex_coord_out;


void main()
{
  // Calculate screen position
  gl_Position = MVP * vec4(position, 1.0);
  cl
  
  // Output other values to fragment shader
  vec4 worldPosition = (M*vec4(position,1.0f));
  
  tex_coord_out = tex_coord_in;
}