#version 440

// Vertex position
layout(location = 0) in vec3 position;
layout (location = 1) in vec4 in_colour;

layout (location = 0) out vec4 out_colour;

// Model view projection matrix
uniform mat4 MVP;

// Offset position
uniform vec3 offsets[800];

void main() 
{
	vec3 offset = offsets[gl_InstanceID];
  // Calculate screen position
  gl_Position = vec4(position+offset, 1.0);
  out_colour = in_colour;
}