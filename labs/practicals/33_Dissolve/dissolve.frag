#version 440

uniform sampler2D tex;
uniform sampler2D dissolve;
uniform sampler2D sahara;

// *********************************
// Declare dissolve factor value
uniform float dissolve_factor;
// *********************************

// Incoming texture coordinates
layout(location = 0) in vec2 tex_coord;
// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Get dissolve value from the dissolve texture
  vec4 dissolve_Value = texture(dissolve, tex_coord);

  // If r component is greater than dissolve factor, discard
  if (dissolve_Value.r > dissolve_factor)
  {
	//discard;
	colour = texture(sahara, tex_coord);
  }
  // *********************************

  // Get texture colour
  else
  {
	colour = texture(tex, tex_coord);
  }
}