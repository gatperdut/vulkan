#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint texIndex;

layout(set = 1, binding = 0) uniform sampler2D texSampler[2];

layout(location = 0) out vec4 outColor;

void main() {
    //outColor = vec4(texIndex - 1.0, 0.0, 0.0, 1.0);
	outColor = vec4(fragColor * texture(texSampler[texIndex], fragTexCoord).rgb, 1.0);
	//outColor = vec4(1.0, 0.0, 0.0, 1.0);
	//outColor = vec4(texture(texSampler, fragTexCoord).rgb, 1.0);
}
