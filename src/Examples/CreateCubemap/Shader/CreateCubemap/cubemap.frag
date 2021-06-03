#version 450
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) in vec3 vert_texcoord;

layout( set = 0, binding = 1 ) uniform samplerCube Cubemap;

layout(location = 0) out vec4 outColor;

void main() {
	outColor = texture( Cubemap, vert_texcoord );
    //outColor = vec4(1.0, 0.0, 0.0, 1.0);
}