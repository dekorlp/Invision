#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 textureCord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler, textureCord * 2.0); //vec4(fragColor, 1.0);
}