#version 450 core

layout(location = 0) out vec4 FragColor;

in VS_OUT {
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
} fs_in;

// Texture uniform
uniform sampler2D u_DefaultTexture;

// Lighting uniforms
uniform vec3 u_CameraPosition;

// Light structure
struct Light {
	vec4 Position;
	vec4 Color;
	vec4 Properties;
};

// Light buffer
layout(std140, binding = 0) uniform LightBuffer {
	int u_LightCount;
	Light u_Lights[32];
};

// Utility functions
vec3 calculateReflection(vec3 viewDir, vec3 normal) {
	return normalize(reflect(-viewDir, normal));
}

float calculateFresnel(vec3 viewDir, vec3 normal, float reflectivity) {
	float cosTheta = clamp(dot(viewDir, normal), 0.0, 1.0);
	return mix(reflectivity, 1.0, pow(1.0 - cosTheta, 5.0));
}

float calculateAttenuation(float distance, float range, float falloff) {
	if (distance > range) return 0.0;
	float attenuation = 1.0 - pow(distance / range, falloff);
	return clamp(attenuation, 0.0, 1.0);
}

float calculateSpotlightFactor(vec3 lightDir, vec3 toLightDir, float innerAngle, float outerAngle) {
	float cosTheta = dot(-lightDir, toLightDir);
	float smoothStep = smoothstep(outerAngle, innerAngle, cosTheta);
	return smoothStep;
}

vec3 calculateLighting(vec3 normal, vec3 viewDir, vec3 worldPos, vec3 baseColor, float roughness, float metallic) {
	vec3 result = vec3(0.0);

	for (int i = 0; i < u_LightCount; ++i) {
		Light light = u_Lights[i];
		vec3 lightDir;
		float attenuation = 1.0;
		float spotlightFactor = 1.0;

		// Directional light
		if (light.Position.w == 0.0) {
			lightDir = normalize(-light.Position.xyz);
		} else { // Point light or spotlight
			lightDir = normalize(light.Position.xyz - worldPos);
			float distance = length(light.Position.xyz - worldPos);
			attenuation = calculateAttenuation(distance, light.Properties.x, light.Properties.y);

			// Spotlight factor
			if (light.Properties.z > 0.0 && light.Properties.w > 0.0) {
				spotlightFactor = calculateSpotlightFactor(light.Position.xyz, lightDir, light.Properties.z, light.Properties.w);
			}
		}

		vec3 radiance = light.Color.rgb * light.Color.a * attenuation * spotlightFactor;

		// BRDF calculations
		vec3 halfDir = normalize(lightDir + viewDir);
		float NDF = pow(clamp(dot(normal, halfDir), 0.0, 1.0), (1.0 - roughness) * 128.0);
		float geometry = clamp(dot(normal, lightDir) * dot(normal, viewDir), 0.0, 1.0);
		float fresnel = calculateFresnel(viewDir, normal, metallic);

		vec3 specular = vec3((NDF * geometry * fresnel) / 
												(4.0 * max(dot(normal, lightDir), 0.0) * max(dot(normal, viewDir), 0.0) + 0.001));
		vec3 diffuse = (1.0 - metallic) * baseColor * max(dot(normal, lightDir), 0.0);

		result += (diffuse + specular) * radiance;
	}

	return result;
}

void main()
{
	vec3 baseColor = texture(u_DefaultTexture, fs_in.TexCoord).rgb * fs_in.Color;
	vec3 normal = normalize(fs_in.Normal);
	vec3 viewDir = normalize(u_CameraPosition - fs_in.WorldPos);

	// Calculate lighting
	vec3 lighting = calculateLighting(normal, viewDir, fs_in.WorldPos, baseColor, fs_in.Roughness, fs_in.Metallic);

	// Add emissive contribution
	vec3 emissive = fs_in.EmissiveColor * fs_in.EmissiveIntensity;
	
	FragColor = vec4(lighting + emissive, fs_in.Alpha);
}