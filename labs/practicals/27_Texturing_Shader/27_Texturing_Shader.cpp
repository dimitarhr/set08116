#include <glm\glm.hpp>
#include <graphics_framework.h>
#include <memory>

using namespace std;
using namespace graphics_framework;
using namespace glm;

mesh m, triangleMesh, cubeMesh;
effect eff;
target_camera cam;
texture tex;
texture texTriangle, fSide;

bool load_content() {
  // Construct geometry object
  geometry geom;
  geometry triangle;
  geometry cube;

  // Create triangle data
  // Positions for geom
  vector<vec3> positions{vec3(-1.0f, 1.0f, 0.0f), vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f),
						 vec3(-1.0f, 1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f)};

  vector<vec3> trianglePositions{ vec3(-1.0f, 1.0f, 0.0f), vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f)};

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Create cube data - twelve triangles triangles
  // Positions
  vector<vec3> cubePositions{
	  // *********************************
	  // Add the position data for triangles here, (6 verts per side)
	  // Front
	  vec3(-1.0f, 1.0f, 0.0f),
	  vec3(-1.0f, -1.0f, 0.0f),
	  vec3(1.0f,-1.0f,0.0f),

	  vec3(-1.0f, 1.0f, 0.0f),
	  vec3(1.0f, -1.0f, 0.0f),
	  vec3(1.0f, 1.0f, 0.0f),

	  // Back
	  vec3(-1.0f,1.0f,-2.0f),
	  vec3(1.0f,1.0f,-2.0f),
	  vec3(1.0f,-1.0f,-2.0f),

	  vec3(-1.0f,1.0f,-2.0f),
	  vec3(1.0f,-1.0f,-2.0f),
	  vec3(-1.0f,-1.0f,-2.0f),

	  // Right
	  vec3(1.0f,1.0f,0.0f),
	  vec3(1.0f,-1.0f,0.0f),
	  vec3(1.0f,-1.0f,-2.0f),

	  vec3(1.0f,1.0f,0.0f),
	  vec3(1.0f,-1.0f,-2.0f),
	  vec3(1.0f,1.0f,-2.0f),

	  // Left
	  vec3(-1.0f,-1.0f,0.0f),
	  vec3(-1.0f,1.0f,0.0f),
	  vec3(-1.0f,1.0f,-2.0f),

	  vec3(-1.0f,-1.0f,0.0f),
	  vec3(-1.0f,1.0f,-2.0f),
	  vec3(-1.0f,-1.0f,-2.0f),

	  // Top
	  vec3(-1.0f,1.0f,0.0f),
	  vec3(1.0f,1.0f,0.0f),
	  vec3(-1.0f,1.0f,-2.0f),

	  vec3(1.0f,1.0f,0.0f),
	  vec3(1.0f,1.0f,-2.0f),
	  vec3(-1.0f,1.0f,-2.0f),
	  /*// Bottom
	  vec3(-1.0f,-1.0f,0.0f),
	  vec3(-1.0f,-1.0f,-2.0f),
	  vec3(1.0f,-1.0f,-2.0f),

	  vec3(1.0f,-1.0f,-2.0f),
	  vec3(1.0f,-1.0f,0.0f),
	  vec3(-1.0f,-1.0f,0.0f)*/
	  // *********************************
  };
  vector<vec2> cube_coords{
  // Front
	vec2(0,1),
	vec2(0,0),
	vec2(1,0),
	vec2(0,1),
	vec2(1,0),
	vec2(1,1),

	// Back
	vec2(1,1),
	vec2(0,1),
	vec2(0,0),
	vec2(1,1),
	vec2(0,0),
	vec2(1,0),

	// Right
	vec2(0,1),
	vec2(0,0),
	vec2(1,0),
	vec2(0,1),
	vec2(1,0),
	vec2(1,1),

	// Left
	vec2(1,0),
	vec2(1,1),
	vec2(0,1),
	vec2(1,0),
	vec2(0,1),
	vec2(0,0),

	// 
  };
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // *********************************
  // Define texture coordinates for geom
  vector<vec2> tex_coords{ vec2(0.0f, 1.0f), vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f,0.0f),vec2(1.0f,1.0f) };
  vector<vec2> triangleTex_coords{ vec2(0.5f, 1.0f), vec2(0.0f, 0.0f), vec2(1.0f, 1.0f)};

  // *********************************
  // Add to the geometry
  geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
  // *********************************
  // Add texture coordinate buffer to geometry
  geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
  // *********************************

  cube.add_buffer(cubePositions,BUFFER_INDEXES::POSITION_BUFFER);
  cube.add_buffer(cube_coords,BUFFER_INDEXES::TEXTURE_COORDS_0);

  triangle.add_buffer(trianglePositions, BUFFER_INDEXES::POSITION_BUFFER);
  triangle.add_buffer(triangleTex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);

  // Create mesh object
  m = mesh(geom);
  triangleMesh = mesh(triangle);
  cubeMesh = mesh(cube);

  // Load in texture shaders here
  eff.add_shader("27_Texturing_Shader/simple_texture.vert", GL_VERTEX_SHADER);
  eff.add_shader("27_Texturing_Shader/simple_texture.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();
  // Load texture "textures/sign.jpg"
  tex = texture("textures/check_6.png");
  texTriangle = texture("textures/sign.jpg");
  fSide = texture("textures/check_1.png");

  // Set camera properties
  cam.set_position(vec3(10.0f, 10.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

  return true;
}

bool update(float delta_time) {

	// Transform
	if (glfwGetKey(renderer::get_window(), 'W')) {
		cubeMesh.get_transform().position -= vec3(0.0f, 0.0f, 5.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'S')) {
		cubeMesh.get_transform().position += vec3(0.0f, 0.0f, 5.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'A')) {
		cubeMesh.get_transform().position -= vec3(5.0f, 0.0f, 0.0f) * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'D')) {
		cubeMesh.get_transform().position += vec3(5.0f, 0.0f, 0.0f) * delta_time;
	}
	// Rotate
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
		cubeMesh.get_transform().rotate(vec3(-pi<float>() * delta_time, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
		cubeMesh.get_transform().rotate(vec3(pi<float>() * delta_time, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT)) {
		cubeMesh.get_transform().rotate(vec3(0.0f, 0.0f, pi<float>() * delta_time));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT)) {
		cubeMesh.get_transform().rotate(vec3(0.0f, 0.0f, -pi<float>() * delta_time));
	}

	// Scale
	if (glfwGetKey(renderer::get_window(), 'O')) {
		cubeMesh.get_transform().scale += vec3(0.1f, 0.1f, 0.1f);
	}
	// Checking if the pressed key is 'P' and if the size is less than 0.0f
	if ((cubeMesh.get_transform().scale.x > 0.0f) && glfwGetKey(renderer::get_window(), 'P')) {
		cubeMesh.get_transform().scale -= vec3(0.1f, 0.1f, 0.1f);
	}

  // Update the camera
  cam.update(delta_time);
  return true;
}

bool render() {
  // Bind effect
  renderer::bind(eff);

  // Bind texture to renderer
  renderer::bind(tex, 0);
  renderer::bind(texTriangle, 1);
  renderer::bind(fSide,2);

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

  // Set the texture value for the shader here
  glUniform1i(eff.get_uniform_location("tex"),0);
  // *********************************

  // Render the mesh
  renderer::render(m);

  // Different mesh and geometry object
   MVP = P * V * translate(M, vec3(3.0,0.0,-2.0));
   glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
	   1,                               // Number of values - 1 mat4
	   GL_FALSE,                        // Transpose the matrix?
	   value_ptr(MVP));                 // Pointer to matrix data

   glUniform1i(eff.get_uniform_location("tex"), 1);
  renderer::render(triangleMesh);

  // The same mesh and geometry object but different texture
  MVP = P * V * translate(M, vec3(0.0, 3.0, 2.0));
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), // Location of uniform
	  1,                               // Number of values - 1 mat4
	  GL_FALSE,                        // Transpose the matrix?
	  value_ptr(MVP));                 // Pointer to matrix data

  glUniform1i(eff.get_uniform_location("tex"), 0);
  renderer::render(m);

  // Showing the cube
  // Get the model transform from the mesh
  mat4 cubeM = cubeMesh.get_transform().get_transform_matrix();
  MVP = P * V * cubeM;
  glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
  glUniform1i(eff.get_uniform_location("tex"), 2);
  renderer::render(cubeMesh);

  return true;
}

void main() {
  // Create application
  app application("27_Texturing_Shader");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}