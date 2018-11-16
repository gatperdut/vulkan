#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

layout(set = 0, location = 0) in vec3 inPosition;
layout(set = 0, location = 1) in vec3 inNormal;
layout(set = 0, location = 2) in vec3 inColor;
layout(set = 0, location = 3) in vec2 inTexCoord;
layout(set = 0, location = 4) in uint inTexIndex;

layout(location = 0) out vec3 normal;
layout(location = 1) out vec3 fragColor;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out uint texIndex;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
	normal = inNormal;
	fragColor = inColor;
	fragTexCoord = inTexCoord;
	texIndex = inTexIndex;
}