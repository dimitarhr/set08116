// Directional light structure
#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT
struct directional_light
{
	vec4 ambient_intensity;
	vec4 light_colour;
	vec3 light_dir;
};
#endif

// A material structure
#ifndef MATERIAL
#define MATERIAL
struct material
{
	vec4 emissive;
	vec4 diffuse_reflection;
	vec4 specular_reflection;
	float shininess;
};
#endif

// Calculates the directional light
vec4 calculate_direction(in directional_light light, in material mat, in vec3 normal, in vec3 view_dir, in vec4 tex_colour)
{
 // *********************************
	// Calculate ambient component
	vec4 ambient = mat.diffuse_reflection * light.ambient_intensity;
	
	// Calculate diffuse component :  (diffuse reflection * light_colour) *  max(dot(normal, light direction), 0)
	float diffuseK = max(dot(normal, light.light_dir),0.0f);
	vec4 diffuse = diffuseK * mat.diffuse_reflection * light.light_colour;
	
	// Calculate normalized half vector 
	vec3 H = normalize(light.light_dir+view_dir);
	
	// Calculate specular component : (specular reflection * light_colour) * (max(dot(normal, half vector), 0))^mat.shininess
	float specularK = pow(max(dot(normal, H),0.0f),mat.shininess);
	vec4 specular = specularK * (mat.specular_reflection * light.light_colour);
 // *********************************
	// Calculate colour to return
	return ((mat.emissive + ambient + diffuse) * tex_colour) + specular;
}
