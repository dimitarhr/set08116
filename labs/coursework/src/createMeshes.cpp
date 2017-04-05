/* createMeshes.cpp
*  Consists of the functions that are required to create the meshes.
*  The global variables in 'main.h' are used.
*  Dimitar Hristov, 40201757, Computer Graphics(SET08116)
*/
#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "createMeshes.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

void setFrameBuffers()
{
	// Create 2 frame buffers - use screen width and height
	frames[0] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	frames[1] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	// Create frame buffer - use screen width and height
	frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	temp_frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	// Create screen quad
	vector<vec3> positions{ vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f) };
	vector<vec2> tex_coords{ vec2(0.0, 0.0), vec2(1.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f) };
	// *********************************
	screen_quad.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	screen_quad.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	screen_quad.set_type(GL_TRIANGLE_STRIP);
}

void createGrass()
{
	float minXZ = -400;
	float maxXZ = 400;
	float minY = 0;
	float maxY = 3;
	//grassMesh.get_transform().translate(vec3(0, 40, 10));
	// Allows creation of random points.  Note range
	default_random_engine randomNumber;
	uniform_real_distribution<float> distXZ(minXZ, maxXZ);
	uniform_real_distribution<float> distY(minY, maxY);

	// INSTANCED ARRAY
	// Randomly generate points
	float randomNumberX;
	float randomNumberY;
	float randomNumberZ;
	for (auto i = 0; i < eggsNumber; ++i) {
		randomNumberX = distXZ(randomNumber);
		randomNumberY = distY(randomNumber);
		randomNumberZ = distXZ(randomNumber);

		offsetArray[i] = (vec3(randomNumberX, randomNumberY-30, randomNumberZ));
	}
}

void createWater()
{
	waterMesh = mesh(geometry_builder::create_plane(1000,1000)); 
	//waterMesh.get_transform().scale = vec3(500);  
	waterMesh.get_transform().translate(vec3(0.0f, 10.0f, 0.0f));
}

void createNormalMapMeshes()
{
	// Create meshes
	normalMapMeshes["earth"] = mesh(geometry_builder::create_sphere(60, 60));
	normalMapMeshes["smallStickBoxLeft"] = mesh(geometry_builder::create_box(vec3(5.0f, 5.0f, 5.0f)));
	normalMapMeshes["smallStickBoxRight"] = mesh(geometry_builder::create_box(vec3(4.0f, 4.0f, 4.0f)));
	normalMapMeshes["smallStickBoxBack"] = mesh(geometry_builder::create_box(vec3(4.5f, 4.5f, 4.5f)));
	normalMapMeshes["smallStickBoxFront"] = mesh(geometry_builder::create_box(vec3(3.0f, 3.0f, 3.0f)));
	normalMapMeshes["sphereLeft"] = mesh(geometry_builder::create_sphere(30, 30, vec3(2.5f, 2.5f, 3.0f)));
	normalMapMeshes["dragonEgg"] = mesh(geometry_builder::create_sphere(50, 50, vec3(3, 2, 2)));

	// Transform meshes
	normalMapMeshes["earth"].get_transform().scale = vec3(2.5f, 2.5f, 2.5f);
	normalMapMeshes["earth"].get_transform().translate(vec3(35.0f, 43.5f, 10.0f));
	normalMapMeshes["earth"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	normalMapMeshes["smallStickBoxLeft"].get_transform().translate(vec3(23.0f, 35.5f, 5.0f));
	normalMapMeshes["smallStickBoxRight"].get_transform().translate(vec3(50.0f, 34.0f, 5.0f));
	normalMapMeshes["smallStickBoxBack"].get_transform().translate(vec3(31.0f, 34.0f, 0.0f));
	normalMapMeshes["smallStickBoxFront"].get_transform().translate(vec3(27.0f, 36.5f, 25.0f));
	normalMapMeshes["sphereLeft"].get_transform().translate(vec3(25.0f, 36.0f, 17.0f));
	//normalMapMeshes["sphereLeft"].get_transform().translate(vec3(0.0f, 0.0f, 0.0f));
	normalMapMeshes["dragonEgg"].get_transform().translate(vec3(35.0f, 47.0f, -30.0f));
}

void createBasicMeshes()
{
	// Create meshes
	basicMeshes["ring"] = mesh(geometry_builder::create_torus(45, 45, 1.0f, 6.5f));
	basicMeshes["moon"] = mesh(geometry_builder::create_sphere(30, 30));
	basicMeshes["ringBase"] = mesh(geometry_builder::create_pyramid());
	basicMeshes["stickBoxLeft"] = mesh(geometry_builder::create_box(vec3(1.0f, 10.0f, 1.0f)));
	basicMeshes["stickBoxRight"] = mesh(geometry_builder::create_box(vec3(2.5f, 10.0f, 1.0f)));
	basicMeshes["stickBoxBack"] = mesh(geometry_builder::create_box(vec3(1.0f, 20.0f, 1.0f)));
	basicMeshes["stickBoxFront"] = mesh(geometry_builder::create_box(vec3(2.5f, 5.0f, 2.5f)));
	basicMeshes["torch"] = mesh(geometry_builder::create_cylinder());
	basicMeshes["dragon"] = mesh(geometry("textures/dragon.obj"));
	basicMeshes["stickBoxSupport"] = mesh(geometry_builder::create_box(vec3(2, 20.0, 2)));

	// Transform meshes
	basicMeshes["moon"].get_transform().translate(vec3(35.0f, 41.5f, 18.0f));
	basicMeshes["moon"].get_transform().rotate(vec3(-half_pi<float>(), 0.0f, quarter_pi<float>() / 2.0f));
	basicMeshes["ring"].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	basicMeshes["ring"].get_transform().translate(vec3(35.0f, 43.5f, 10.0f));
	basicMeshes["ringBase"].get_transform().scale = vec3(5.0f, 5.0f, 5.0f);
	basicMeshes["ringBase"].get_transform().translate(vec3(35.0f, 34.0f, 10.0f));
	basicMeshes["stickBoxLeft"].get_transform().translate(vec3(23.0f, 39.0f, 10.0f));
	basicMeshes["stickBoxRight"].get_transform().translate(vec3(50.0f, 37.0f, 10.0f));
	basicMeshes["stickBoxBack"].get_transform().translate(vec3(35.0f, 42.0f, 0.0f));
	basicMeshes["stickBoxFront"].get_transform().translate(vec3(32.0f, 37.5f, 25.0f));
	basicMeshes["torch"].get_transform().rotate(vec3(0.0f, 0.0f, half_pi<float>()));
	basicMeshes["dragon"].get_transform().scale = vec3(10);
	basicMeshes["dragon"].get_transform().translate(vec3(28.0f, 43.0f, 15.0f));
	basicMeshes["dragon"].get_transform().rotate(vec3(0.0f, -quarter_pi<float>(), 0.0f));
	basicMeshes["stickBoxSupport"].get_transform().translate(vec3(-44.0f, 32.9f, 65.0f));
}

void createShadowMeshes()
{
	// Create meshes
	shadow_geom["shadowWall"] = mesh(geometry_builder::create_box(vec3(3, 15, 96)));
	shadow_geom["miniWall"] = mesh(geometry_builder::create_box(vec3(1, 7, 10)));
	//shadow_geom["floorPlane"] = mesh(geometry_builder::create_plane(8,100,true));
	shadow_geom["floorPlane"] = mesh(geometry_builder::create_box(vec3(8, 2, 100)));
	shadow_geom["stickBoxFront"] = mesh(geometry_builder::create_box(vec3(1.0f, 10.0f, 1.0f)));
	shadow_geom["bigEgg"] = mesh(geometry_builder::create_sphere(50, 50, vec3(3, 2, 2)));
	shadow_geom["smallEgg"] = mesh(geometry_builder::create_sphere(50, 50, vec3(2, 1, 1)));

	// Transform meshes
	shadow_geom["shadowWall"].get_transform().translate(vec3(-46.5f, 50.5f, 15.0f));
	shadow_geom["miniWall"].get_transform().translate(vec3(-40.5f, 46.5f, 15.0f));
	shadow_geom["stickBoxFront"].get_transform().translate(vec3(-40.5f, 48.0f, 35.0f));
	//shadow_geom["floorPlane"].get_transform().translate(vec3(-46.0f, 43.0f, 65.0f));
	shadow_geom["floorPlane"].get_transform().translate(vec3(-44.0f, 42.0f, 17.0f));
	shadow_geom["bigEgg"].get_transform().translate(vec3(-40.5f, 44.5f, 0.0f));
	shadow_geom["bigEgg"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f));
	shadow_geom["smallEgg"].get_transform().translate(vec3(-38.5f, 46.5f, 15.0f));
	shadow_geom["smallEgg"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 15.0f));
}

void createHierarchicalMeshes()
{
	// Create meshes (From outter most to center)
	hierarchicalMesh[0] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 10.0f));
	hierarchicalMesh[1] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 9.0f));
	hierarchicalMesh[2] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 8.0f));
	hierarchicalMesh[3] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 7.0f));
	hierarchicalMesh[4] = mesh(geometry_builder::create_torus(20, 20, 0.5f, 6.0f));

	// Transform meshes
	hierarchicalMesh[0].get_transform().translate(vec3(35.0f, 47.0f, -30.0f));
	hierarchicalMesh[0].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	hierarchicalMesh[1].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	hierarchicalMesh[2].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	hierarchicalMesh[3].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
	hierarchicalMesh[4].get_transform().rotate(vec3(half_pi<float>(), 0.0f, 0.0f));
}

void generate_terrain(geometry &geom, const texture &height_map, unsigned int width, unsigned int depth, float height_scale)
{
	// Contains our position data
	vector<vec3> positions;
	// Contains our normal data
	vector<vec3> normals;
	// Contains our texture coordinate data
	vector<vec2> tex_coords;
	// Contains our texture weights
	vector<vec4> tex_weights;
	// Contains our index data
	vector<unsigned int> indices;

	// Extract the texture data from the image
	glBindTexture(GL_TEXTURE_2D, height_map.get_id());
	auto data = new vec4[height_map.get_width() * height_map.get_height()];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (void *)data);

	// Determine ratio of height map to geometry
	float width_point = static_cast<float>(width) / static_cast<float>(height_map.get_width());
	float depth_point = static_cast<float>(depth) / static_cast<float>(height_map.get_height());

	// Point to work on
	vec3 point;

	// Create all the points and scale them properly
	// Part 1 - Iterate through each point, calculate vertex and add to vector
	cout << "Width: " << height_map.get_width() << endl;
	cout << "Height: " << height_map.get_height()<<endl;
	for (int x = 0; x < height_map.get_width(); ++x) {
		// Calculate x position of point
		point.x = -(width / 2.0f) + (width_point * static_cast<float>(x));

		for (int z = 0; z < height_map.get_height(); ++z) {
			// *********************************
			// Calculate z position of point
			point.z = -(depth / 2.0f) + (depth_point*static_cast<float>(z));
			// *********************************
			// Y position based on red component of height map data
			point.y = data[(z * height_map.get_width()) + x].y * height_scale;
			// Add point to position data
			positions.push_back(point);
		}
	}

	// Create the triangles from all the points by using the indices
	// Part 1 - Add index data
	for (unsigned int x = 0; x < height_map.get_width() - 1; ++x) {
		for (unsigned int y = 0; y < height_map.get_height() - 1; ++y) {
			// Get four corners of patch
			unsigned int top_left = (y * height_map.get_width()) + x;
			unsigned int top_right = (y * height_map.get_width()) + x + 1;
			// *********************************
			unsigned int bottom_left = ((y + 1)*height_map.get_width()) + x;
			unsigned int bottom_right = ((y + 1)*height_map.get_height()) + x + 1;
			// *********************************
			// Push back indices for triangle 1 (tl,br,bl)
			indices.push_back(top_left);
			indices.push_back(bottom_right);
			indices.push_back(bottom_left);
			// Push back indices for triangle 2 (tl,tr,br)
			// *********************************
			indices.push_back(top_left);
			indices.push_back(top_right);
			indices.push_back(bottom_right);
			// *********************************
		}
	}

	// Resize the normals buffer
	normals.resize(positions.size());

	// Take the three sides of every triangle and calculate the sides
	// Then with the cross product get the normal for this triangle (sides)
	// Part 2 - Calculate normals for the height map
	cout << "Indices: " << indices.size() << endl;
	for (unsigned int i = 0; i < indices.size() / 3; ++i) {
		// Get indices for the triangle
		auto idx1 = indices[i * 3];
		auto idx2 = indices[i * 3 + 1];
		auto idx3 = indices[i * 3 + 2];

		// Calculate two sides of the triangle
		vec3 side1 = positions[idx1] - positions[idx3];
		vec3 side2 = positions[idx1] - positions[idx2];

		// Normal is normal(cross product) of these two sides
		// *********************************
		vec3 n = normalize(cross(side2, side1));

		// Add to normals in the normal buffer using the indices for the triangle
		normals[idx1] = n;
		normals[idx2] = n;
		normals[idx3] = n;
		// *********************************
	}

	// Normalize all the normals
	for (auto &n : normals) {
		// *********************************
		n = normalize(n);
		// *********************************
	}

	// Part 3 - Add texture coordinates for geometry
	for (unsigned int x = 0; x < height_map.get_width(); ++x) {
		for (unsigned int z = 0; z < height_map.get_height(); ++z) {
			tex_coords.push_back(vec2(width_point * x, depth_point * z));
		}
	}

	// Calculate the weight from the y data
	// 0.25 - four weights/textures
	// 0.0,0.15,0.5,0.9 - arbitrary points
	// Part 4 - Calculate texture weights for each vertex
	for (unsigned int x = 0; x < height_map.get_width(); ++x) {
		for (unsigned int z = 0; z < height_map.get_height(); ++z) {
			// Calculate tex weight
			vec4 tex_weight(clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.0f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.15f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.5f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.9f) / 0.25f, 0.0f, 1.0f));

			// *********************************
			// Sum the components of the vector
			double total = tex_weight.x + tex_weight.y + tex_weight.z + tex_weight.w;
			// Divide weight by sum
			tex_weight /= total;
			// Add tex weight to weights
			tex_weights.push_back(tex_weight);
			// *********************************
		}
	}

	// Add necessary buffers to the geometry
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
	geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	geom.add_buffer(tex_weights, BUFFER_INDEXES::TEXTURE_COORDS_1);
	geom.add_index_buffer(indices);

	// Delete data
	delete[] data;
}

void createTerrain()
{
	// Geometry to load into
	geometry geom;

	// Load height map 
	texture height_map("textures/mountain_map.png");

	// Generate terrain
	generate_terrain(geom, height_map, 800, 800, 100.0f);

	// Use geometry to create terrain mesh
	terrainMesh = mesh(geom);
}