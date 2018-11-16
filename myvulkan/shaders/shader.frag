#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, location = 0) in vec3 fragPos;
layout(set = 0, location = 1) in vec3 normal;
layout(set = 0, location = 2) in vec3 fragColor;
layout(set = 0, location = 3) in vec2 fragTexCoord;
layout(set = 0, location = 4) flat in uint texIndex;

layout(set = 0, binding = 1) uniform sampler2D texSamplers[5];

layout(location = 0) out vec4 outColor;


void main() {
	vec3 lightPos = vec3(0.0, 5.0, 0.0);
	vec3 normNormal = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = min(max(dot(normNormal, lightDir), 0.0), 0.95);

	outColor = vec4((diff + 0.05) * fragColor * texture(texSamplers[texIndex], fragTexCoord).rgb, 1.0);
}
