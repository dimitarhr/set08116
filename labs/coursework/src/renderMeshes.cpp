/* renderMeshes.cpp
*  Consists of the functions that are required to render the meshes.
*  The global variables in 'main.h' are used.
*  Dimitar Hristov, 40201757, Computer Graphics(SET08116)
*/
#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "renderMeshes.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

void renderNormalMapMesh()
{
	// Bing effect
	renderer::bind(normalMappingEff);

	// Bind lights
	renderer::bind(dirLight, "light");
	renderer::bind(pointLight, "pointLight");
	renderer::bind(spots, "spots");

	for (auto &item : normalMapMeshes) {
		// Gets the mesh
		auto geometryItem = item.second;
		// Gets the name of the mesh
		string geometryName = item.first;
		// Normal matrix
		auto N = geometryItem.get_transform().get_normal_matrix();
		// Create MVP matrix
		auto M = geometryItem.get_transform().get_transform_matrix();
		auto V = cams[cameraIndex]->get_view();
		auto P = cams[cameraIndex]->get_projection();
		auto MVP = P * V * M;

		// Set MVP matrix uniform
		glUniformMatrix4fv(normalMappingEff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		// Set M matrix uniform
		glUniformMatrix4fv(normalMappingEff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));

		// Set N matrix uniform
		glUniformMatrix3fv(normalMappingEff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));

		// Bind material
		renderer::bind(geometryItem.get_material(), "mat");

		// Bind texture
		if (textures.find(geometryName) != textures.end())
		{
			renderer::bind(textures[geometryName], 0);
		}
		else
		{
			renderer::bind(textures["surroundings"], 0);
		}

		// Bind normal map
		if (normal_maps.find(geometryName) != normal_maps.end())
		{
			renderer::bind(normal_maps[geometryName], 1);
		}
		else
		{
			renderer::bind(normal_maps["surroundings"], 1);
		}

		// Set the texture uniform value
		glUniform1i(normalMappingEff.get_uniform_location("tex"), 0);

		// Set the normal_map uniform value
		glUniform1i(normalMappingEff.get_uniform_location("normal_map"), 1);

		// Set the viewer position uniform value
		glUniform3fv(normalMappingEff.get_uniform_location("eye_pos"), 1, value_ptr(cams[cameraIndex]->get_position()));

		// Render geometry
		renderer::render(geometryItem);
	}
}

void renderBasicMesh()
{
	// Bind effect
	renderer::bind(basicEff);

	// Bind lights
	renderer::bind(dirLight, "light");
	renderer::bind(pointLight, "pointLight");
	renderer::bind(spots, "spots");

	for (auto &item : basicMeshes) {
		// Gets the mesh
		auto geometryItem = item.second;
		// Gets the name of the mesh
		string geometryName = item.first;
		// Normal matrix
		auto N = geometryItem.get_transform().get_normal_matrix();
		// Create MVP matrix
		auto M = geometryItem.get_transform().get_transform_matrix();
		auto V = cams[cameraIndex]->get_view();
		auto P = cams[cameraIndex]->get_projection();
		auto MVP = P * V * M;

		// Set MVP matrix uniform
		glUniformMatrix4fv(basicEff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		// Set M matrix uniform
		glUniformMatrix4fv(basicEff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));

		// Set N matrix uniform
		glUniformMatrix3fv(basicEff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));

		// Bind material
		renderer::bind(geometryItem.get_material(), "mat");

		// Bind texture
		if (textures.find(geometryName) != textures.end())
		{
			renderer::bind(textures[geometryName], 0);
		}
		else
		{
			renderer::bind(textures["surroundings"], 0);
		}

		// Set the texture uniform value
		glUniform1i(basicEff.get_uniform_location("tex"), 0);

		// Set the viewer position uniform value
		glUniform3fv(basicEff.get_uniform_location("eye_pos"), 1, value_ptr(cams[cameraIndex]->get_position()));

		// Render geometry
		renderer::render(geometryItem);
	}
}

void renderShadowMesh()
{
	// Set render target to shadow map
	renderer::set_render_target(shadowMap);
	// Clear depth buffer bit
	glClear(GL_DEPTH_BUFFER_BIT);
	// Set render mode to cull face
	glCullFace(GL_FRONT);
	// Create a new projection matrix for the light
	mat4 LightProjectionMat = perspective<float>(90.f, renderer::get_screen_aspect(), 0.1f, 1000.f);

	// Bind shader
	renderer::bind(basicEff);

	// Render meshes
	for (auto &e : shadow_geom) {
		// Gets every mesh
		auto m = e.second;
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		// View matrix taken from shadow map
		auto V = shadowMap.get_view();
		// Calculate the MVP matrix
		auto MVP = LightProjectionMat * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(basicEff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		// Render mesh
		renderer::render(m);
	}

	// Set render target to frame buffer
	renderer::set_render_target(frame);

	// Set cull face to back
	glCullFace(GL_BACK);

	// Bind shader
	renderer::bind(shadows_eff);

	// Render meshes
	for (auto &e : shadow_geom) {
		// Gets every mesh
		auto m = e.second;
		// Gets the name of every mesh
		string geometryName = e.first;
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cams[cameraIndex]->get_view();
		auto P = cams[cameraIndex]->get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(shadows_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		// Set M matrix uniform
		glUniformMatrix4fv(shadows_eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		// Set N matrix uniform
		glUniformMatrix3fv(shadows_eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));

		// Set light transform
		auto lM = m.get_transform().get_transform_matrix();
		auto lV = shadowMap.get_view();
		auto lightMVP = LightProjectionMat * lV * lM;
		glUniformMatrix4fv(shadows_eff.get_uniform_location("lightMVP"), 1, GL_FALSE, value_ptr(lightMVP));
		// Bind material
		renderer::bind(m.get_material(), "mat");
		// Bind spot light
		renderer::bind(spots[1], "spot");
		// Bind texture
		if (textures.find(geometryName) != textures.end())
		{
			renderer::bind(textures[geometryName], 0);
		}
		else
		{
			renderer::bind(textures["surroundings"], 0);
		}
		// Set tex uniform
		glUniform1i(shadows_eff.get_uniform_location("tex"), 0);
		// Set eye position
		glUniform3fv(shadows_eff.get_uniform_location("eye_pos"), 1, value_ptr(cams[cameraIndex]->get_position()));
		// Bind shadow map texture
		renderer::bind(shadowMap.buffer->get_depth(), 1);
		glUniform1i(shadows_eff.get_uniform_location("shadow_map"), 1);
		// Render mesh
		renderer::render(m);
	}
}

void renderSkyBox()
{
	// Disable depth test,depth mask,face culling
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	// Bind effect
	renderer::bind(sky_eff);

	// Calculate MVP for the skybox
	auto M = skybox.get_transform().get_transform_matrix();
	auto V = cams[cameraIndex]->get_view();
	auto P = cams[cameraIndex]->get_projection();
	auto MVP = P * V * M;
	renderer::bind(cube_map, 0);

	// Set MVP matrix uniform
	glUniformMatrix4fv(sky_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

	// Set cubemap uniform
	glUniform1i(sky_eff.get_uniform_location("cubemap"), 0);

	// Render skybox
	renderer::render(skybox);

	// Enable depth test,depth mask,face culling
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
}

void renderHierarchicalMeshes()
{
	// Bind effect
	renderer::bind(basicEff);

	// Bind lights
	renderer::bind(dirLight, "light");
	renderer::bind(pointLight, "pointLight");
	renderer::bind(spots, "spots");

	// Get PV
	const auto PV = cams[cameraIndex]->get_projection() * cams[cameraIndex]->get_view();
	// Set the texture value for the shader here
	glUniform1i(basicEff.get_uniform_location("tex"), 0);
	// Find the lcoation for the MVP uniform
	const auto loc = basicEff.get_uniform_location("MVP");

	// Render meshes
	for (size_t i = 0; i < hierarchicalMesh.size(); i++) {
		// Normal matrix
		auto N = hierarchicalMesh[i].get_transform().get_normal_matrix();
		// SET M to be the mesh transform matrix
		auto M = hierarchicalMesh[i].get_transform().get_transform_matrix();
		// Set M matrix uniform
		glUniformMatrix4fv(basicEff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		// Set N matrix uniform
		glUniformMatrix3fv(basicEff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));
		// Apply the heirarchy chain
		for (size_t j = i; j > 0; j--)
		{
			M = hierarchicalMesh[j - 1].get_transform().get_transform_matrix() * M;
		}
		// Bind material
		renderer::bind(hierarchicalMesh[i].get_material(), "mat");
		// Set MVP matrix uniform
		glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(PV * M));
		// Bind texture to renderer
		renderer::bind(textures["ring"], 0);
		// Render mesh
		renderer::render(hierarchicalMesh[i]);
	}
}
// Renders a piece of geometry
void renderModified(const geometry &geom) throw(...) {
	assert(geom.get_array_object() != 0);
	// Check renderer is running
	//assert(renderer::_instance->_running);
	// Bind the vertex array object for the
	glBindVertexArray(geom.get_array_object());
	// Check for any OpenGL errors
	if (CHECK_GL_ERROR) {
		// Display error
		std::cerr << "ERROR - rendering geometry" << std::endl;
		std::cerr << "Could not bind vertex array object" << std::endl;
		// Throw exception
		throw std::runtime_error("Error rendering geometry");
	}
	// If there is an index buffer then use to render
	if (geom.get_idx_buffer() != 0) {
		// Bind index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.get_idx_buffer());
		// Check for error
		if (CHECK_GL_ERROR) {
			std::cerr << "ERROR - rendering geometry" << std::endl;
			std::cerr << "Could not bind index buffer" << std::endl;
			// Throw exception
			throw std::runtime_error("Error rendering geometry");
		}
		// Draw elements
		glDrawElementsInstanced(geom.get_type(), geom.get_index_count(), GL_UNSIGNED_INT, nullptr, 500);
		// Check for error
		if (CHECK_GL_ERROR) {
			// Display error
			std::cerr << "ERROR - rendering geometry" << std::endl;
			std::cerr << "Could not draw elements from indices" << std::endl;
			// Throw exception
			throw std::runtime_error("Error rendering geometry");
		}
	}
	else {
		// Draw arrays
		glDrawArraysInstanced(geom.get_type(), 0, geom.get_vertex_count(), 500);
		// Check for error
		if (CHECK_GL_ERROR) {
			std::cerr << "ERROR - rendering geometry" << std::endl;
			std::cerr << "Could not draw arrays" << std::endl;
			// Throw exception
			throw std::runtime_error("Error rendering geometry");
		}
	}
}

void renderGrass() 
{
	// Bind effect
	renderer::bind(grass_eff);
		// Normal matrix
		auto N = grassMesh.get_transform().get_normal_matrix();
		// Create MVP matrix
		auto M = grassMesh.get_transform().get_transform_matrix();
		auto V = cams[cameraIndex]->get_view();
		auto P = cams[cameraIndex]->get_projection();
		auto MVP = P * V * M;

		// Set MVP matrix uniform
		glUniformMatrix4fv(grass_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		glUniform3fv(grass_eff.get_uniform_location("offsets"),500 , value_ptr(offsetArray[0]));

		// Render geometry
		renderModified(grassMesh.get_geometry());
}


void renderTerrain()
{
	renderer::bind(terrain_eff);
	auto M = terrainMesh.get_transform().get_transform_matrix();
	auto V = cams[cameraIndex]->get_view();
	auto P = cams[cameraIndex]->get_projection();
	auto MVP = P * V * M;
	// Set MVP matrix uniform
	glUniformMatrix4fv(terrain_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	// Set M matrix uniform
	glUniformMatrix4fv(terrain_eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
	// Set N matrix uniform
	glUniformMatrix3fv(terrain_eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(terrainMesh.get_transform().get_normal_matrix()));
	// *********************************
	// Set eye_pos uniform to camera position
	glUniform3fv(terrain_eff.get_uniform_location("eye_pos"), 1, value_ptr(cams[cameraIndex]->get_position()));
	// *********************************
	//Bind Terrian Material
	renderer::bind(terrainMesh.get_material(), "mat");
	// Bind Light
	renderer::bind(dirLight, "light");
		// Bind Tex[0] to TU 0, set uniform
		renderer::bind(terrainTex[0], 0);
		glUniform1i(terrain_eff.get_uniform_location("tex[0]"), 0);
		// *********************************
		//Bind Tex[1] to TU 1, set uniform
		renderer::bind(terrainTex[1], 1);
		glUniform1i(terrain_eff.get_uniform_location("tex[1]"), 1);
		// Bind Tex[2] to TU 2, set uniform
		renderer::bind(terrainTex[2], 2);
		glUniform1i(terrain_eff.get_uniform_location("tex[2]"), 2);
		// Bind Tex[3] to TU 3, set uniform
		renderer::bind(terrainTex[3], 3);
		glUniform1i(terrain_eff.get_uniform_location("tex[3]"), 3);
	// *********************************
	// Render terrain
	renderer::render(terrainMesh);
}

void renderEdges()
{
	// Set render target to the edge frame
	renderer::set_render_target(temp_frame);
	// Clear frame
	renderer::clear();
	// Bind Tex effect
	renderer::bind(edge_eff);
	// MVP is now the identity matrix
	auto MVP = mat4(1);
	glUniformMatrix4fv(edge_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	// Bind texture from frame buffer
	renderer::bind(frame.get_frame(), 1);
	// Set the tex uniform
	glUniform1i(edge_eff.get_uniform_location("tex"), 1);
	// Set inverse width Uniform
	glUniform1f(edge_eff.get_uniform_location("screen_width"), renderer::get_screen_width());
	// Set inverse height Uniform
	glUniform1f(edge_eff.get_uniform_location("screen_height"), renderer::get_screen_height());
	// Render the screen quad    
	renderer::render(screen_quad);
}

void renderSepia()
{
	// Set render target to the edge frame
	renderer::set_render_target(temp_frame);
	// Clear frame
	renderer::clear();
	// Bind Tex effect
	renderer::bind(sepia_eff);
	// MVP is now the identity matrix
	auto MVP = mat4(1);
	// Set MVP matrix uniform
	glUniformMatrix4fv(sepia_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	// Bind texture from frame buffer
	renderer::bind(frame.get_frame(), 1);
	// Set the tex uniform
	glUniform1i(sepia_eff.get_uniform_location("tex"), 1);
	// Render the screen quad
	renderer::render(screen_quad);
}

void renderMotionBlur()
{
	// Set render target to current frame
	renderer::set_render_target(frames[current_frame]);
	// Clear frame
	renderer::clear();
	// Bind motion blur effect
	renderer::bind(motion_blur_eff);
	// MVP is now the identity matrix
	auto MVP = mat4(1.0);
	// Set MVP matrix uniform
	glUniformMatrix4fv(motion_blur_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	// Bind tempframe to TU 0.
	renderer::bind(frame.get_frame(), 0);
	// Bind frames[(current_frame + 1) % 2] to TU 1.
	renderer::bind(frames[(current_frame + 1) % 2].get_frame(), 1);
	// Set tex uniforms
	glUniform1i(motion_blur_eff.get_uniform_location("tex"), 0);
	glUniform1i(motion_blur_eff.get_uniform_location("previous_frame"), 1);
	// Set blend factor (0.9f)
	glUniform1f(motion_blur_eff.get_uniform_location("blend_factor"), 0.9f);
	// Render screen quad
	renderer::render(screen_quad);
}

void renderMask()  
{
	// Set render target back to the screen 
	renderer::set_render_target(); 
	// Bind Tex effect
	renderer::bind(mask_eff);
	// MVP is now the identity matrix
	auto MVP = mat4(1);
	// Set MVP matrix uniform
	glUniformMatrix4fv(mask_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	if (edgeDetection == 1 || sepia == 1)
	{
		// Bind texture from frame buffer to TU 0
		renderer::bind(temp_frame.get_frame(), 0);
	}
	else
	{
		// Bind texture from frame buffer to TU 0
		if (motionBlur == 1)
		{
			renderer::bind(frames[current_frame].get_frame(), 0);
		}
		else
		{
			renderer::bind(frame.get_frame(), 0);
		}
	}
	// Set the tex uniform, 0
	glUniform1i(mask_eff.get_uniform_location("tex"), 0);
	if (screenMode == 0)
	{
		// Bind alpha texture to TU, 1
		renderer::bind(originalMap, 1);
		// Set the tex uniform, 1
		glUniform1i(mask_eff.get_uniform_location("alpha_map"), 1);
	}
	else
	{
		// Bind alpha texture to TU, 1
		renderer::bind(alpha_map, 1);
		// Set the tex uniform, 1
		glUniform1i(mask_eff.get_uniform_location("alpha_map"), 1);
	}
	// Render the screen quad
	renderer::render(screen_quad);
}