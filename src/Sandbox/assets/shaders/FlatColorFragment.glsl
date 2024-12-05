#version 330 core

// Input from the vertex shader
in vec4 v_Color;

// Output to the framebuffer
out vec4 FragColor;

void main() {
	// Use the color passed from the vertex shader
	FragColor = v_Color;
}