#version 450

layout(location = 0) out vec4 color;

float LinearizeDepth(float depth)
{
  float n = 0.1; // camera z near
  float f = 25.0; // camera z far
  float z = depth;
  return (2.0 * n) / (f + n - z * (f - n));	
}

void main() {	
	//color = vec4(gl_FragCoord.z, 0.0, 0.0, 1.0);
	//color = vec4(1.0 - LinearizeDepth(gl_FragCoord.z), 0.0, 0.0, 1.0);
	//color = vec4(1.0, 0.0, 0.0, 1.0);
	//color = vec4(gl_FragCoord.z, 0.0, 0.0, 1.0);
}