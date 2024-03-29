#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 normal;

void main() {
	mat3 normalMatrix = mat3(transpose(inverse(ubo.view * ubo.model)));
    normal =  vec3(vec4(normalMatrix * inNormal, 0.0));
	gl_Position = ubo.view * ubo.model * vec4(inPosition, 1.0);
}