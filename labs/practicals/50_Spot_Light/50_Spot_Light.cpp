#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
effect eff;
texture tex;
target_camera cam;
spot_light light; 

double cursor_x = 0.0;
double cursor_y = 0.0;  

// before load_content
bool initialise() {
	// *********************************
	// Set input mode - hide the cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Capture initial mouse position
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	// *********************************
	return true;
}

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

  // *********************************
  // Set lighting values
  // Position (-25, 10, -10)
  light.set_position(vec3(-25,10,-10));
  // Light colour white
  light.set_light_colour(vec4(1.0f,1.0f,1.0f,1.0f));
  // Light direction to forward and down (normalized)
  light.set_direction(vec3(0.0f,0.0f,-1.0f));
  // Set range to 20
  light.set_range(20);
  // Set power to 1
  light.set_power(1);
  // Load in shaders
  eff.add_shader("50_Spot_Light/spot.vert", GL_VERTEX_SHADER);
  eff.add_shader("50_Spot_Light/spot.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();
  // *********************************
  // Set camera properties
  //cam.set_position(vec3(50.0f, 10.0f, 50.0f));
  cam.set_position(vec3(-50.0f, 10.0f, -50.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}

bool update(float delta_time) {
  // Range of the spot light
  static float range = 20.0f;

  // *********************************
  // WSAD to move point light
  if (glfwGetKey(renderer::get_window(), 'W')) {
	  light.move(vec3(0.0f, 0.0f, 20.0f) * delta_time);
  }
  if (glfwGetKey(renderer::get_window(), 'S')) {
	  light.move(vec3(0.0f, 0.0f, -20.0f) * delta_time);
  }
  if (glfwGetKey(renderer::get_window(), 'A')) {
	  light.move(vec3(-20.0f, 0.0f, 0.0f) * delta_time);
  }
  if (glfwGetKey(renderer::get_window(), 'D')) {
	  light.move(vec3(20.0f, 0.0f, 0.0f) * delta_time);
  }
  // O and P to change range
  if (glfwGetKey(renderer::get_window(), 'O')) {
	  range += 10.0f;
  }
  if (glfwGetKey(renderer::get_window(), 'P')) {
	  range -= 10.0f;
  }
  // Cursor keys to rotate camera on X and Y axis
  //light.rotate(vec3(0,0, half_pi<float>()*delta_time));

  // The ratio of pixels to rotation - remember the fov
  static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
  static double ratio_height =
	  (quarter_pi<float>() *
	  (static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) /
	  static_cast<float>(renderer::get_screen_height());

  double current_x = 0;
  double current_y = 0;
  double delta_x;
  double delta_y;
  // *********************************
  // Get the current cursor position
  glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
  // Calculate delta of cursor positions from last frame
  delta_x = current_x - cursor_x;
  delta_y = current_y - cursor_y;
  // Multiply deltas by ratios - gets actual change in orientation
  delta_x = delta_x * ratio_width;
  delta_y = delta_y * ratio_height;

  // Update cursor pos
  cursor_x = current_x;
  cursor_y = current_y;
  
  light.rotate(vec3(delta_x,delta_y,0.0f));







  // *********************************

  // Set range
  light.set_range(range);

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
	// Bind light
	renderer::bind(light, "spot");
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
  app application("50_Spot_Light");
  // Set load content, update and render methods
  application.set_initialise(initialise);
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}