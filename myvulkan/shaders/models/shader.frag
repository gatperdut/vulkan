#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, location = 0) in vec3 fragPos;
layout(set = 0, location = 1) in vec3 normal;
layout(set = 0, location = 2) in vec3 fragColor;
layout(set = 0, location = 3) in vec2 fragTexCoord;
layout(set = 0, location = 4) flat in uint texIndex;
layout(set = 0, location = 5) in vec4 shadowCoord[3];

struct Light {
	vec4 pos;
	vec4 color;
};

layout(set = 0, binding = 0) uniform LightUniformBufferObject {
	Light lights[3];
};

layout(set = 0, binding = 2) uniform sampler2D shadowmap[3];

layout(set = 1, binding = 1) uniform sampler2D texSamplers[3];

layout(location = 0) out vec4 outColor;

void main() {

	vec3 lightColor = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < lights.length(); i++) {
		vec4 normShadowCoord = shadowCoord[i] / shadowCoord[i].w;
		vec3 lightPos = vec3(lights[i].pos);
		vec3 lightDir = normalize(lightPos - fragPos);
		vec3 normNormal = normalize(normal);
		float bias = max(0.05 * (1.0 - dot(normNormal, lightDir)), 0.005);  
		if (texture(shadowmap[i], normShadowCoord.xy).r > normShadowCoord.z - bias) {
			float diff = min(max(dot(normNormal, lightDir), 0.0), 0.95);
			lightColor += vec3(diff) * vec3(lights[i].color);
		}
	}

	lightColor = clamp(lightColor, vec3(0.05, 0.05, 0.05), vec3(1.0, 1.0, 1.0));

	outColor = vec4(lightColor * fragColor * texture(texSamplers[texIndex], fragTexCoord).rgb, 1.0);
}
