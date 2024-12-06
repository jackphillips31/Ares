#type vertex
#version 330 core

// Input attributes from the vertex buffer
layout(location = 0) in vec3 a_Position;	// Position (x, y, z)
layout(location = 1) in vec4 a_Color;		// Color (r, g, b, a)

// Uniform for the ViewProjection matrix
uniform mat4 u_ViewProjectionMatrix;

// Output to the fragment shader
out vec4 v_Color;

void main() {
	// Pass the color to the fragment shader
	v_Color = a_Color;

	// Apply the ViewProjection matrix to the vertex position
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

// Input from the vertex shader
in vec4 v_Color;

// Output to the framebuffer
out vec4 FragColor;

void main() {
	// Use the color passed from the vertex shader
	FragColor = v_Color;
}