#version 450 core

// Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
// Instanced attributes
layout(location = 3) in mat4 a_Transform;
layout(location = 7) in vec3 a_Color;
layout(location = 8) in float a_Alpha;
layout(location = 9) in float a_Roughness;
layout(location = 10) in float a_Metallic;
layout(location = 11) in float a_Reflectivity;
layout(location = 12) in vec3 a_EmissiveColor;
layout(location = 13) in float a_EmissiveIntensity;
//layout(location = 14) in int a_ReceiveShadows;
//layout(location = 15) in int a_CastShadows;
//layout(location = 16) in int a_Wireframe;

uniform mat4 u_ViewProjection;

// Output to fragment shader
out VS_OUT {
	vec3 WorldPos;
	vec2 TexCoord;
	vec3 Normal;
	vec3 Color;
	float Alpha;
	float Roughness;
	float Metallic;
	float Reflectivity;
	vec3 EmissiveColor;
	float EmissiveIntensity;
} vs_out;

void main()
{
	// Calculate world position
	vec4 worldPosition = a_Transform * vec4(a_Position, 1.0);
	gl_Position = u_ViewProjection * worldPosition;

	// Transform normal to world space
	mat3 normalMatrix = transpose(inverse(mat3(a_Transform)));
	vec3 worldNormal = normalize(normalMatrix * a_Normal);

	// Pass properties to fragment shader
	vs_out.WorldPos = worldPosition.xyz;
	vs_out.TexCoord = a_TexCoord;
	vs_out.Normal = worldNormal;
	vs_out.Color = a_Color;
	vs_out.Alpha = a_Alpha;
	vs_out.Roughness = a_Roughness;
	vs_out.Metallic = a_Metallic;
	vs_out.Reflectivity = a_Reflectivity;
	vs_out.EmissiveColor = a_EmissiveColor;
	vs_out.EmissiveIntensity = a_EmissiveIntensity;
}