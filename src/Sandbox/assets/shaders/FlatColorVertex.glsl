#version 330 core

// Input attributes from the vertex buffer
layout(location = 0) in vec3 a_Position;	// Position (x, y, z)
layout(location = 1) in vec4 a_Color;		// Color (r, g, b, a)

// Output to the fragment shader
out vec4 v_Color;

void main() {
	// Pass the color to the fragment shader
	v_Color = a_Color;

	// Set the vertex position (z is ignored for 2D rendering)
	gl_Position = vec4(a_Position, 1.0);
}