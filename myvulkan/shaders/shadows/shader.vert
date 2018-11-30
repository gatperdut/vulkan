#version 450
#extension GL_ARB_separate_shader_objects : enable

struct LightSpace {
	mat4 projectionView;
};

layout(set = 0, binding = 0) uniform UniformBufferObject {
	LightSpace lightSpace;
} ubo;

layout(set = 1, binding = 0) uniform UBOModelsMatrices {
	mat4 proj;
	mat4 view;
	mat4 model;
} uboMatrices;

layout(set = 0, location = 0) in vec3 inPosition;

out gl_PerVertex {
    vec4 gl_Position;
};


void main() {
    gl_Position = ubo.lightSpace.projectionView * uboMatrices.model * vec4(inPosition, 1.0);
	gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;

}