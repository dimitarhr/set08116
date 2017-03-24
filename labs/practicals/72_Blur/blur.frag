#version 430 core

// Incoming frame data
uniform sampler2D tex;

// 1.0f / screen width
uniform float inverse_width;
// 1.0f / screen height
uniform float inverse_height;

// Surrounding pixels to sample and their scale
const vec4 samples[4] = vec4[4](vec4(-1.0, 0.0, 0.0, 0.25), vec4(1.0, 0.0, 0.0, 0.25), vec4(0.0, 1.0, 0.0, 0.25),
                                vec4(0.0, -1.0, 0.0, 0.25));
// Edge detection
const vec4 samplesEdge[6] = vec4[6](vec4(-1.0, 1.0, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 2.0), vec4(1.0, 1.0, 0.0, 1.0),
                                vec4(-1.0, -1.0, 0.0, -1.0), vec4(0.0, -1.0, 0.0,-2.0), vec4(1.0, -1.0, 0.0, -1.0));

// SharpEdge detection
const vec4 samplesSharpEdge[5] = vec4[5](vec4(-1.0, 0.0, 0.0, -2.0/3.0), vec4(0.0, 1.0, 0.0, -2.0/3.0), vec4(1.0, 0.0, 0.0, -2.0/3.0),
                                vec4(0.0, -1.0, 0.0, -2.0/3.0), vec4(0.0, 0.0, 0.0,11.0/3.0));

// Gaussian One detection
const vec4 samplesGaussianOne[7] = vec4[7](vec4(-3.0, 0.0, 0.0, 1.0/64.0), vec4(-2.0, 0.0, 0.0, 6.0/64.0), vec4(-1.0, 0.0, 0.0, 15.0/64.0),
                                vec4(0.0, 0.0, 0.0, 20.0/64.0), vec4(1.0, 0.0, 0.0, 15.0/64.0), vec4(2.0, 0.0, 0.0, 6.0/64.0), vec4(3.0, 0.0, 0.0, 1.0/64.0));

// Gaussian Two detection
const vec4 samplesGaussianTwo[7] = vec4[7](vec4(0.0, -3.0, 0.0, 1.0/64.0), vec4(0.0, -2.0, 0.0, 6.0/64.0), vec4(0.0, -1.0, 0.0, 15.0/64.0),
                                vec4(0.0, 0.0, 0.0, 20.0/64.0), vec4(0.0, 1.0, 0.0, 15.0/64.0), vec4(0.0, 2.0, 0.0, 6.0/64.0), vec4(0.0, 3.0, 0.0, 1.0/64.0));
// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Start with colour as black
  colour = vec4(0.0f,0.0f,0.0f,1.0f);
  // Loop through each sample vector
 /* for (int i = 0; i < 5; i++)
  {
    // Calculate tex coord to sample
	vec2 uv = tex_coord + vec2(samplesEdge[i].x * inverse_width, samplesEdge[i].y * inverse_height);
    // Sample the texture and scale appropriately
    // - scale factor stored in w component
	vec4 tempColour = texture(tex,uv);
	tempColour *= samplesEdge[i].w;
	colour += tempColour;
	}*/

	for (int i = 0; i < 7; i++)
	{
		// Calculate tex coord to sample
		vec2 uv = tex_coord + vec2(samplesGaussianOne[i].x * inverse_width, samplesGaussianOne[i].y * inverse_height);
		// Sample the texture and scale appropriately
		// - scale factor stored in w component
		vec4 tempColour = texture(tex,uv);
		tempColour *= samplesGaussianOne[i].w;
		colour += tempColour;
	}

	for (int i = 0; i < 7; i++)
	{
		// Calculate tex coord to sample
		vec2 uv = tex_coord + vec2(samplesGaussianTwo[i].y * inverse_height , samplesGaussianTwo[i].x * inverse_width);
		// Sample the texture and scale appropriately
		// - scale factor stored in w component
		vec4 tempColour = texture(tex,uv);
		tempColour *= samplesGaussianTwo[i].w;
		colour += tempColour;
	}

  // Ensure alpha is 1.0
  colour.a = 1.0;
  // *********************************
}