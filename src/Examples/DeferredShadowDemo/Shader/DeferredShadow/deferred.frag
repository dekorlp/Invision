#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D texAlbedo;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texPosition;
layout(binding = 3) uniform sampler2D shadowMap;

layout(location = 1) in vec2 textureCord;

layout(location = 0) out vec4 outColor;

layout(binding = 4) uniform OptionsUniformBuffer {
	int option;
} uob;

layout(binding = 5) uniform LightUniformBuffer {
	vec3 lightPos;
	vec3 lightColor;
	vec3 viewPos;
} lub;

float LinearizeDepth(float depth)
{
  float n = 1.0; // camera z near
  float f = 128.0; // camera z far
  float z = depth;
  return (2.0 * n) / (f + n - z * (f - n));	
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightShadowDir = normalize(light.position - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightShadowDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main() {

	switch(uob.option)
	{
		case 1:
		{
			outColor = texture(texAlbedo, textureCord);
			break;
		}
		case 2:
		{		
			outColor = texture(texNormal, textureCord);
			break;
		}
		case 3:
		{
			outColor = texture(texPosition, textureCord);
			break;
		}
		case 4: // lightning
		{
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
		case 5: // lightning and shadows
		{
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
			}
			break;
			
		case 6: // debug shadow Map
		{
			float depthValue = texture(shadowMap, textureCord).r;
			outColor = vec4(vec3(1.0-LinearizeDepth(depthValue)), 1.0);
			break;
		}
	}
	

   
}