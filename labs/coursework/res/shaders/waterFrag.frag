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

// Directional light information
uniform directional_light light;

// Normal map to sample from
uniform sampler2D refractionTexture;

// DuDv map used for distortion of the water
// Every pixel is red or green which can be represented as a 2D vector. It is used to add distortion to the water
uniform sampler2D dudvMap;

uniform sampler2D depthMap;

uniform sampler2D normal_map;

uniform float moveFactor;

// Cubemap texture
uniform samplerCube cubemap;

// Incoming data
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 toCameraVecor;
layout(location = 4) in vec3 reflectionVector;
layout(location = 5) in vec4 clipSpace;


// Outgoing colour
layout(location = 0) out vec4 colour;

// Strength of the distortion
const float waveStrength = 0.02;
// Light characteristics
const float shineDamper = 20.0;
const float reflectivity = 0.1;

void main() 
{	

	// Get the screen space by diving by w. Divide by 2 and add 0.5 to move the origin of the texture down in the left corner
	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
	
	// Refraction texture coordinates
	vec2 refractionTexCoords = vec2(ndc.x, ndc.y);
	vec3 reflectionTexCoords = reflectionVector;

	float near = 0.1;
	float far = 1000.0;

	// Sampling the depth map
	float depth = texture(depthMap, refractionTexCoords).r; // The depth information is stored in the red component

	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	
	depth = gl_FragCoord.z;
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	float waterDepth = floorDistance - waterDistance;

	// Distortion
	// The distortion is stored in the Red and Green values
	// * 2.0 - 1.0 -> Convert them to be between -1 and 1 to be more realistic
	vec2 distortedTexCoords = texture(dudvMap, vec2(tex_coord.x + moveFactor, tex_coord.y)).rg*0.5;
	distortedTexCoords = tex_coord + vec2(distortedTexCoords.x, distortedTexCoords.y+moveFactor);
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

	// Add the distortion to the Refraction texture coordinates
	refractionTexCoords += totalDistortion;
	reflectionTexCoords += vec3(totalDistortion * 50.0, 0.0);
	refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999); // The texture coordinates shouldn't go neither too high or too low

	// Sample the refraction texture
	vec4 refractionColour = texture(refractionTexture, refractionTexCoords);

	// View vector
	vec3 viewVector = normalize(toCameraVecor);

	// Fresnel effect
	// We assume that the water normal is pointing straight upwards
	float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));
	// Makes the water a bit less transparent
	refractiveFactor = pow(refractiveFactor, 0.5);
	
	// The reflection is just water colour
	//vec4 reflectionColour = vec4(0.25, 0.64, 0.97, 1.0);
	vec4 reflectionColour = texture(cubemap, reflectionTexCoords);

	// Sampling the normal map
	vec4 normalMapColour = texture(normal_map, distortedTexCoords);
	vec3 normal = vec3(normalMapColour.r * 2.0 - 1.0, normalMapColour.b, normalMapColour.g * 2.0 - 1.0);
	normal = normalize(normal);

	// Light calculations
	vec3 reflectedLight = reflect(normalize(-light.light_dir), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = light.light_colour.rgb * specular * reflectivity * clamp(waterDepth/2.0, 0.5, 1.0);
	
	// Mix the two colours
	colour = mix(reflectionColour, refractionColour, refractiveFactor);
	colour = mix(colour, vec4(0.6f, 0.37f, 0.33f, 1.0f), 0.2) + vec4(specularHighlights, 0.0); // Add the light effect to the final colour
	// The depth value shows how transparent the water is
	colour.a = clamp(waterDepth, 0.5, 1.0);
}