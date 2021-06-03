#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;

layout( location = 0 ) out vec3 vert_texcoord;


void main() {
	vec3 pos = mat3(ubo.model) * inPosition.xyz;
    gl_Position = (ubo.proj * vec4(inPosition, 0.0)).xyzz;
	vert_texcoord = inPosition.xyz;
}