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
		gl_Position = MVP * (gl_in[i].gl_Position);
		// *********************************
		colour_out = vec4(0.0, 1.0, 0.0, 1.0);
		EmitVertex();
		// *********************************
	}
	EndPrimitive();
	
	/*float addX = sin((gl_in[0].gl_Position).x);
	float addZ = sin((M * gl_in[0].gl_Position).z);*/

  // *********************************
  // Emit a copy of the triangle moved by negative offset
  // Offset triangle needs to be blue
	for (int i = 0; i < 3; ++i) 
	{
		gl_Position = MVP * (gl_in[i].gl_Position + vec4(m,0.05,m, 0));
		// *********************************
		colour_out = vec4(1.0, 0.0, 0.0, 1.0);
		EmitVertex();
		// *********************************
	}
	EndPrimitive();

	for (int i = 0; i < 3; ++i) 
	{
		gl_Position = MVP * (gl_in[i].gl_Position + vec4(m+0.7,0.05,m+0.7, 0));
		// *********************************
		colour_out = vec4(0.0, 0.0, 1.0, 1.0);
		EmitVertex();
		// *********************************
	}
	EndPrimitive();

	for (int j = 1;j<40;j++){
		for (int i = 0; i < 3; ++i) 
		{
			gl_Position = MVP * (gl_in[i].gl_Position + vec4(m+0.5,0.05,m+0.5, 0));
			// *********************************
			colour_out = vec4(0.0, 0.0, 0.0, 1.0);
			EmitVertex();
			// *********************************
		}
		EndPrimitive();
	}

  // *********************************
}