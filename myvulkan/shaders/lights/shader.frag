#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform Light_Attrs {
	vec4 pos;
	vec4 color;
} light_Attrs;

layout(location = 0) out vec4 outColor;

void main() {
	outColor = light_Attrs.color;
}
