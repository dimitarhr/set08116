#version 440

// This shader requires direction.frag, point.frag and spot.frag

// Directional light structure
#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT
struct directional_light {
  vec4 ambient_intensity;
  vec4 light_colour;
  vec3 light_dir;
};
#endif

// Point light information
#ifndef POINT_LIGHT
#define POINT_LIGHT
struct point_light {
  vec4 light_colour;
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};
#endif

// Spot light data
#ifndef SPOT_LIGHT
#define SPOT_LIGHT
struct spot_light {
  vec4 light_colour;
  vec3 position;
  vec3 direction;
  float constant;
  float linear;
  float quadratic;
  float power;
};
#endif

// A material structure
#ifndef MATERIAL
#define MATERIAL
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};
#endif

// Directional light information
uniform directional_light light;
// Point lights being used in the scene
uniform point_light pointLight;
// Spot lights being used in the scene
uniform spot_light spots[5];
// Material of the object being rendered
uniform material mat;
// Position of the eye
uniform vec3 eye_pos;
// Texture to sample from
uniform sampler2D tex;
// Normal map to sample from
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D depthMap;
uniform sampler2D normal_map;
uniform float moveFacotr;
uniform vec3 lightColour;

// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 toCameraVecor;
// Incoming texture coordinate
layout(location = 5) in vec4 clipSpace;


// Outgoing colour
layout(location = 0) out vec4 colour;

void main() 
{	

	// Get the screen space by diving by w. Divide by 2 and add 0.5 to move the origin of the texture down in the left corner
	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
	
	// Refraction texture coordinates
	vec2 refractionTexCoords = vec2(ndc.x, ndc.y);

	// Sample the refraction texture
	vec4 refractionColour = texture(refractionTexture, refractionTexCoords);
	
	// The reflection is just water colour
	vec4 reflectionColour = vec4(0.25, 0.64, 0.87, 1.0);
	
	// Mix the two colours
	colour = mix(reflectionColour, refractionColour, 0.5);
}