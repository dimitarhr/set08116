#version 440

// Point light information
struct point_light {
  vec4 light_colour;
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};

// Spot light data
struct spot_light {
  vec4 light_colour;
  vec3 position;
  vec3 direction;
  float constant;
  float linear;
  float quadratic;
  float power;
};

// Material data
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

// Point lights being used in the scene
uniform point_light points[4];
// Spot lights being used in the scene
uniform spot_light spots[5];
// Material of the object being rendered
uniform material mat;
// Position of the eye (camera)
uniform vec3 eye_pos;
// Texture to sample from
uniform sampler2D tex;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 1) in vec3 normal;
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

// Point light calculation
vec4 calculate_point(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir,
                     in vec4 tex_colour) {
  // *********************************
  // Get distance between point light and vertex
  float d = distance(point.position,position);
  
  // Calculate attenuation factor
  float attenuationFac = (point.constant + point.linear*d + point.quadratic*d*d);
  
  // Calculate light colour
  vec4 light_colour = point.light_colour/attenuationFac;

  // Calculate light dir
  vec3 light_dir = normalize(point.position-position);

  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient

   // Calculate diffuse component
  // Calculate k
  float diffuseK = max(dot(normal, light_dir),0.0f);
  
  // Calculate diffuse
  vec4 diffuse = diffuseK * mat.diffuse_reflection * light_colour;
  
  // Calculate half vector between view_dir and light_dir
  vec3 H = normalize(light_dir+view_dir);

  // Calculate specular component
  // Calculate k
  float specularK = pow(max(dot(normal, H),0.0f),mat.shininess);
  
  // Calculate specular
  vec4 specular = specularK * (mat.specular_reflection * light_colour);

  // Calculate primary colour component
  // Set primary
  vec4 primary = mat.emissive + diffuse;
  
  // Set secondary
  vec4 secondary = specular;

  // Calculate final colour - remember alpha
  primary.a = 1.0f;
  secondary.a = 1.0f;

  colour = primary*tex_colour + secondary;

  // *********************************
  return colour;
  //return colour = vec4(1.0f,0.0f,0.0f,1.0f);
}

// Spot light calculation
vec4 calculate_spot(in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir,
                    in vec4 tex_colour) {
  // *********************************
  // Calculate direction to the light
      vec3 light_dir = normalize(spot.position - position);

  // Calculate distance to light
      float d = distance(spot.position,position);

  // Calculate attenuation value
      float attenuationFac = (spot.constant + spot.linear*d + spot.quadratic*d*d);

  // Calculate spot light intensity
      float light_intensity = pow(max(dot((-1)*spot.direction,light_dir),0.0f),spot.power);

  // Calculate light colour
      vec4 light_colour = light_intensity / attenuationFac * spot.light_colour;

  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient
  // Calculate diffuse component
  // Calculate k
  float diffuseK = max(dot(normal, light_dir),0.0f);
  
  // Calculate diffuse
  vec4 diffuse = diffuseK * mat.diffuse_reflection * light_colour;
  
  // Calculate half vector between view_dir and light_dir
  vec3 H = normalize(light_dir+view_dir);

  // Calculate specular component
  // Calculate k
  float specularK = pow(max(dot(normal, H),0.0f),mat.shininess);
  
  // Calculate specular
  vec4 specular = specularK * (mat.specular_reflection * light_colour);

  
  //if(dot(spot.direction,light_dir) < 0)
  // return colour = vec4(0.0f,1.0f,0.0f,1.0f);

  // Calculate primary colour component
  // Set primary
  vec4 primary = mat.emissive + diffuse;
  
  // Set secondary
  vec4 secondary = specular;

  // Calculate final colour - remember alpha
  primary.a = 1.0f;
  secondary.a = 1.0f;

  colour = primary*tex_colour + secondary;
  // *********************************
  return colour;
  //return colour = vec4(0.0f,1.0f,0.0f,1.0f);
}

void main() {

  colour = vec4(0.0, 0.0, 0.0, 1.0);
  // *********************************
  // Calculate view direction
      vec3 view_dir = normalize(eye_pos-position);

  // Sample texture
	  vec4 tex_colour = texture(tex, tex_coord);

  // Sum point lights
	for (int i = 0; i < 4; ++i)
		colour += calculate_point(points[i], mat, position, normal, view_dir, tex_colour);
		
  // Sum spot lights
  for (int i = 0; i < 5; ++i)
		colour += calculate_spot(spots[i], mat, position, normal, view_dir, tex_colour);
  // *********************************
  colour.w = 1.0f;
}