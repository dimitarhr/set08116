#version 440

// Model view projection matrix
uniform mat4 MVP;

// Incoming value for the position
layout(location = 0) in vec3 position;
uniform vec4 colourOne;
uniform  vec4 colourTwo;

layout(location = 0) out vec4 outputColour;

// Main vertex shader function
void main() {
  // Calculate screen position of vertex
  gl_Position = MVP * vec4(position, 1.0);

  // Adding the two colours together
  outputColour = colourOne + colourTwo;
}