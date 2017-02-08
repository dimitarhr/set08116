#version 440

// The model matrix
uniform mat4 M;

// The transformation matrix
uniform mat4 MVP;

// The normal matrix
uniform mat3 N;

// Material colour
uniform vec4 material_colour;

// Shininess of the object
uniform float shininess;

// Light colour
uniform vec4 light_colour;

// Direction of the light
uniform vec3 light_dir;

// Position of the camera
uniform vec3 eye_pos;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 2) in vec3 normal;

// Outgoing vertex colour
layout(location = 0) out vec4 vertex_colour;

void main() {
  // *********************************
  // Calculate position
     gl_Position = MVP * vec4(position, 1.0);
  
  // Transform the normal
    vec3 transformed_normal = N * normal;
  
  // Calculate world position
    vec4 world_Position = M * vec4(position, 1.0);
  
  // Calculate view direction
	vec4 viewDir = normalize(eye_pos-world_Position);
  
  // Calculate half vector between view_dir and light_dir
	vec4 H = normalize(light_dir+viewDir);
  
  // Calculate k
    float k = pow(max(dot(vec4(transformed_normal,1), H), 0.0), shininess);
  
  // Calculate specular
    vec4 diffuse = k * (material_colour * light_colour);
  
  // Ensure alpha is 1
    diffuse.a = 1.0;
  
  // Output vertex colour - just diffuse
    vertex_colour = diffuse;
  // *********************************
}