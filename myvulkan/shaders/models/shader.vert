#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform UBOModelsMatrices {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

struct LightSpace {
	mat4 projectionView;
};

layout(set = 0, binding = 1) uniform UBOLightsMatrix {
	LightSpace lightSpace[10];
} lightUbo;

layout(set = 0, location = 0) in vec3 inPosition;
layout(set = 0, location = 1) in vec3 inNormal;
layout(set = 0, location = 2) in vec3 inColor;
layout(set = 0, location = 3) in vec2 inTexCoord;
layout(set = 0, location = 4) in uint inTexIndex;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec3 fragColor;
layout(location = 3) out vec2 fragTexCoord;
layout(location = 4) out uint texIndex;
layout(location = 5) out vec4 shadowCoord;

out gl_PerVertex {
    vec4 gl_Position;
};

const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0
);

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
	fragPos = vec3(ubo.model * vec4(inPosition, 1.0));
	normal = inNormal;
	fragColor = inColor;
	fragTexCoord = inTexCoord;
	texIndex = inTexIndex;
	shadowCoord = (lightUbo.lightSpace[0].projectionView * ubo.model) * vec4(inPosition, 1.0);
}