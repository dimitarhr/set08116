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
layout(location = 2) out vec2 tex_coord;
layout(location = 3) out vec3 toCameraVector;
layout(location = 5) out vec4 clipSpace;


void main()
{
  // Calculate screen position
  clipSpace = MVP * vec4(position, 1.0); // The clip space coordinates of the vertex
  gl_Position = clipSpace;
  
  // Output other values to fragment shader
  vec4 worldPosition = (M*vec4(position,1.0f));
  
  tex_coord = tex_coord_in * 0.06;

  // Vector pointing towards the camera
  toCameraVector = cameraPostion - worldPosition.xyz;
}