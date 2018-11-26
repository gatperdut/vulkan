#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, location = 0) in vec3 fragPos;
layout(set = 0, location = 1) in vec3 normal;
layout(set = 0, location = 2) in vec3 fragColor;
layout(set = 0, location = 3) in vec2 fragTexCoord;
layout(set = 0, location = 4) flat in uint texIndex;
layout(set = 0, location = 5) in vec4 shadowCoord;

struct Light {
	vec4 pos;
	vec4 color;
};

layout(set = 0, binding = 0) uniform LightUniformBufferObject {
	Light lights[10];
};

layout(set = 0, binding = 2) uniform sampler2D shadowmap;

layout(set = 1, binding = 1) uniform sampler2D texSamplers[5];

layout(location = 0) out vec4 outColor;

void main() {
	vec4 normShadowCoord = shadowCoord;
	
	const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0
);

	//normShadowCoord = biasMat * normShadowCoord;
	
	normShadowCoord /= shadowCoord.w;

	vec3 lightColor = vec3(0.0, 0.0, 0.0);

	//outColor = vec4(texture(texSamplers[texIndex], fragTexCoord).rgb * 0.1, 1.0) +  vec4(vec3(1.0 - texture(shadowmap, normShadowCoord.xy).r), 1.0);
	//return;

	//for (int i = 0; i < lights.length(); i++) {
		if (texture(shadowmap, normShadowCoord.xy).r > normShadowCoord.z - 0.005) {
			vec3 lightPos = vec3(lights[0].pos);
			vec3 normNormal = normalize(normal);
			vec3 lightDir = normalize(lightPos - fragPos);
			float diff = min(max(dot(normNormal, lightDir), 0.0), 0.95);
			lightColor += vec3(diff) * vec3(lights[0].color);
		}
	//}

	lightColor = clamp(lightColor, vec3(0.05, 0.05, 0.05), vec3(1.0, 1.0, 1.0));

	outColor = vec4(lightColor * fragColor * texture(texSamplers[texIndex], fragTexCoord).rgb, 1.0);
}
