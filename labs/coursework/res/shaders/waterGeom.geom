#version 440

// Model view projection matrix
uniform mat4 MVP;

// Model view projection matrix
uniform float m;


// Offset position
//uniform vec3 offset;

// Layout of incoming data
layout(triangles) in;
// Layout of outgoing data
layout(triangle_strip, max_vertices = 10000) out;

// Outgoing colour for the vertex
layout(location = 0) out vec4 colour_out;

void main() {



  // Emit a copy of the triangle moved by offset
  // Offset triangle needs to be green
	for (int i = 0; i < 3; ++i) 
	{
		//float new_y = gl_in[i].gl_Position.y * sin(m.y)*2;
		//gl_Position = MVP * (gl_in[i].gl_Position.x,new_y,gl_in[i].gl_Position.z,0.0);
		gl_Position = MVP * (gl_in[i].gl_Position);
		// *********************************
		colour_out = vec4(0.0, 1.0, 0.0, 1.0);
		EmitVertex();
		// *********************************
	}
	EndPrimitive();
	

  // *********************************
}