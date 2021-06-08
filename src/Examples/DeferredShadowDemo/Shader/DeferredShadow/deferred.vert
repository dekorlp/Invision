#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 1) out vec2 textureCord;
layout(location = 2) out mat4 fragPosLightSpace;

layout(binding = 4) uniform LightUniformBuffer {
	vec3 lightPos;
	vec3 lightColor;
	vec3 viewPos;
} lub;

void main() {
	textureCord = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(textureCord * 2.0f - 1.0f, 0.0f, 1.0f);
}