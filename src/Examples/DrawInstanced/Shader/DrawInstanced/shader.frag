#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform LightUniformBuffer {
	vec3 lightPos;
	vec3 lightColor;
	vec3 viewPos;
} lub;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

void main() {

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lub.lightColor;


	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(lub.lightPos - fragPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lub.lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(lub.viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lub.lightColor;

	vec4 result = vec4((ambient + diffuse + specular), 1.0) * texture(texSampler, fragTexCoord);
    outColor = result;
}