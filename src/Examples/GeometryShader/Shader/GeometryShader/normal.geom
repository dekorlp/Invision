#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 normal[];




const float MAGNITUDE = 0.4;

void GenerateLine(int index)
{
    gl_Position = ubo.proj * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = ubo.proj * (gl_in[index].gl_Position + 
                                vec4(normal[index], 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main() {
   GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}