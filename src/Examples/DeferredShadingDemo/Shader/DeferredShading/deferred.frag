#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D texAlbedo;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texPosition;

layout(location = 1) in vec2 textureCord;

layout(location = 0) out vec4 outColor;

layout(binding = 3) uniform OptionsUniformBuffer {
	int option;
} uob;

layout(binding = 4) uniform LightUniformBuffer {
	vec3 lightPos;
	vec3 lightColor;
	vec3 viewPos;
} lub;

void main() {

	switch(uob.option)
	{
		case 1:
			outColor = texture(texAlbedo, textureCord);
			break;
		case 2:
			outColor = texture(texNormal, textureCord);
			break;
		case 3:
			outColor = texture(texPosition, textureCord);
			break;
	}

   
}