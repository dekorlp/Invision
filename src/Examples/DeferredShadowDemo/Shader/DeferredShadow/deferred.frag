#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D texAlbedo;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texPosition;
layout(binding = 3) uniform sampler2D shadowMap;

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
		case 4: // lightning and shadows
			// src: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
		
			vec3 FragPos = texture(texPosition, textureCord).rgb;
		
			vec3 color = texture(texAlbedo, textureCord).rgb;
			vec3 normal = texture(texNormal, textureCord).rgb;
			//ambient
			vec3 ambient = 0.15 * color;
			//diffuse
			vec3 lightDir = normalize(lub.lightPos - FragPos);
			float diff = max(dot(lightDir, normal), 0.0);
			vec3 diffuse = diff * lub.lightColor;
			// specular
			vec3 viewdir = normalize(lub.viewPos - FragPos);
			float spec = 0.0;
			vec3 halfwayDir = normalize(lightDir + viewdir);
			spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
			vec3 specular = spec * lub.lightColor;
			// calculate light and shadow
			vec3 lightning = (ambient + diffuse + specular) * color; 
		
		
		
			outColor = vec4(lightning, 1.0);
			break;
	}

   
}