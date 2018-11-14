#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint texIndex;

layout(set = 0, binding = 1) uniform sampler2D texSamplers[5];

layout(location = 0) out vec4 outColor;

void main() {
	outColor = vec4(fragColor * texture(texSamplers[texIndex], fragTexCoord).rgb, 1.0);
}
