#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect eff; 
texture tex;
target_camera cam;
vector<point_light> points(4);
vector<spot_light> spots(5);

bool load_content() {
  // Create plane mesh
  meshes["plane"] = mesh(geometry_builder::create_plane());

  // Create scene
  meshes["box"] = mesh(geometry_builder::create_box());
  meshes["tetra"] = mesh(geometry_builder::create_tetrahedron());
  meshes["pyramid"] = mesh(geometry_builder::create_pyramid());
  meshes["disk"] = mesh(geometry_builder::create_disk(20));
  meshes["cylinder"] = mesh(geometry_builder::create_cylinder(20, 20));
  meshes["sphere"] = mesh(geometry_builder::create_sphere(20, 20));
  meshes["torus"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 5.0f));

  // Transform objects
  meshes["box"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
  meshes["box"].get_transform().translate(vec3(-10.0f, 2.5f, -30.0f));
  meshes["tetra"].get_transform().scale = vec3(4.0f, 4.0f, 4.0f);
  meshes["tetra"].get_transform().translate(vec3(-30.0f, 10.0f, -10.0f));
  meshes["pyramid"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
  meshes["pyramid"].get_transform().translate(vec3(-10.0f, 7.5f, -30.0f));
  meshes["disk"].get_transform().scale = vec3(3.0f, 1.0f, 3.0f);
  meshes["disk"].get_transform().translate(vec3(-10.0f, 11.5f, -30.0f));
  meshes["disk"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
  meshes["cylinder"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
  meshes["cylinder"].get_transform().translate(vec3(-25.0f, 2.5f, -25.0f));
  meshes["sphere"].get_transform().scale = vec3(2.5f, 2.5f, 2.5f);
  meshes["sphere"].get_transform().translate(vec3(-25.0f, 10.0f, -25.0f));
  meshes["torus"].get_transform().translate(vec3(-25.0f, 10.0f, -25.0f));
  meshes["torus"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));

  // *********************************
  // Set materials
  // - all emissive is black
  // - all specular is white
  // - all shininess is 25
  // Red box
  material objectMaterial;
  objectMaterial.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  objectMaterial.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  objectMaterial.set_shininess(25.0f);
  objectMaterial.set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));
  meshes["box"].set_material(objectMaterial);
  // Green tetra
  objectMaterial.set_diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f));
  meshes["tetra"].set_material(objectMaterial);
  // Blue pyramid
  objectMaterial.set_diffuse(vec4(0.0f, 0.0f, 1.0f, 1.0f));
  meshes["pyramid"].set_material(objectMaterial);
  // Yellow disk
  objectMaterial.set_diffuse(vec4(1.0f, 1.0f, 0.0f, 1.0f));
  meshes["disk"].set_material(objectMaterial);
  // Magenta cylinder
  objectMaterial.set_diffuse(vec4(1.0f, 0.0f, 1.0f, 1.0f));
  meshes["cylinder"].set_material(objectMaterial);
  // Cyan sphere
  objectMaterial.set_diffuse(vec4(0.0f, 1.0f, 1.0f, 1.0f));
  meshes["sphere"].set_material(objectMaterial);
  // White torus
  objectMaterial.set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  meshes["torus"].set_material(objectMaterial);
  // *********************************

  // Load texture
  tex = texture("textures/checker_White.gif");

  // Set lighting values
  // *********************************
  // Point 0, Position (-25, 5, -15)
  // Red, 20 range
  points[0].set_position(vec3(-25,5,-15));
  points[0].set_light_colour(vec4(1.0f,0.0f,0.0f,1.0f));
  points[0].set_range(20.0f);
   
  // Point 1, Position (-25, 5, -35)
  // Red,20 range  
  points[1].set_position(vec3(-25, 5, -35));
  points[1].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
  points[1].set_range(20.0f);
   
  // Point 2,Position (-10, 5, -15)
  // Red,20 range
  points[2].set_position(vec3(-10, 5, -15));
  points[2].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
  points[2].set_range(20.0f);

  // Point 3,Position (-10, 5, -35)
  // Red,20 range
  points[3].set_position(vec3(-10, 5, -35));
  points[3].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
  points[3].set_range(20.0f);

  // Spot 0, Position (-25, 10, -15)
  // Green, Direction (1, -1, -1) normalized
  // 20 range,0.5 power
  spots[0].set_position(vec3(-25,10,-15));
  spots[0].set_light_colour(vec4(0.0f,1.0f,0.0f,1.0f));
  spots[0].set_direction(normalize(vec3(1,-1,-1)));
  spots[0].set_range(20); 
  spots[0].set_power(0.5f);  
   
  // Spot 1,Position (-25, 10, -35)
  // Green,Direction (1, -1, 1) normalized
  // 20 range,0.5 power
  spots[1].set_position(vec3(-25, 10, -35));
  spots[1].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
  spots[1].set_direction(normalize(vec3(1, -1, 1)));
  spots[1].set_range(20);
  spots[1].set_power(0.5f);

  // Spot 2,Position (-10, 10, -15)
  // Green,Direction (-1, -1, -1) normalized
  // 20 range,0.5 power
  spots[2].set_position(vec3(-10, 10, -15));
  spots[2].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
  spots[2].set_direction(normalize(vec3(-1, -1, -1)));
  spots[2].set_range(20);
  spots[2].set_power(0.5f);
   
  // Spot 3,Position (-10, 10, -35) 
  // Green,Direction (-1, -1, 1) normalized
  // 20 range,0.5 power
  spots[3].set_position(vec3(-10, 10, -35));
  spots[3].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));
  spots[3].set_direction(normalize(vec3(-1, -1, 1))); 
  spots[3].set_range(20);
  spots[3].set_power(0.5f);   

  // Spot 4,Position (-17.5, 15, -25)
  // Blue,Direction (0, -1, 0)
  // 30 range,1.0 power
  spots[4].set_position(vec3(-17.5f, 15, -25));
  spots[4].set_light_colour(vec4(0.0f, 0.0f, 1.0f, 1.0f));
  spots[4].set_direction(normalize(vec3(0, -1, 0)));
  spots[4].set_range(30);
  spots[4].set_power(1.0f);
  
  // Load in shaders
  eff.add_shader("51_Multiple_Lights/multi-light.vert", GL_VERTEX_SHADER);
  eff.add_shader("51_Multiple_Lights/multi-light.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();
  // *********************************
  // Set camera properties
  cam.set_position(vec3(50.0f, 10.0f, 50.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}
  
bool update(float delta_time) {
  if (glfwGetKey(renderer::get_window(), '1')) {
    cam.set_position(vec3(50, 10, 50));
  }
  if (glfwGetKey(renderer::get_window(), '2')) {
    cam.set_position(vec3(-50, 10, 50));
  }
  if (glfwGetKey(renderer::get_window(), '3')) {
    cam.set_position(vec3(-50, 10, -50));
  }
  if (glfwGetKey(renderer::get_window(), '4')) {
    cam.set_position(vec3(50, 10, -50));
  }
  // Rotate the sphere
  meshes["sphere"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);

  cam.update(delta_time);

  return true;
}

bool render() {
  // Render meshes
  for (auto &e : meshes) {
    auto m = e.second;
	auto N = m.get_transform().get_normal_matrix();
    // Bind effect
    renderer::bind(eff);
    // Create MVP matrix
    auto M = m.get_transform().get_transform_matrix();
    auto V = cam.get_view();
    auto P = cam.get_projection();
    auto MVP = P * V * M;
    // Set MVP matrix uniform
    glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
                       1,                               // Number of values - 1 mat4
                       GL_FALSE,                        // Transpose the matrix?
                       value_ptr(MVP));                 // Pointer to matrix data
    // *********************************
	// Set M matrix uniform
	glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
	// Set N matrix uniform - remember - 3x3 matrix
	glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));
	// Bind material
	renderer::bind(m.get_material(), "mat");
    // Bind point lights
	renderer::bind(points,"points");
    // Bind spot lights 
	renderer::bind(spots,"spots");
	// Bind texture
	renderer::bind(tex, 1);          
	// Set tex uniform
	glUniform1i(eff.get_uniform_location("tex"), 1);
	// Set eye position- Get this from active camera
	glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));
	// Render mesh
	renderer::render(m); 
    // *********************************
  }

  return true;
}

void main() {
  // Create application
  app application("51_Multiple_Lights");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}