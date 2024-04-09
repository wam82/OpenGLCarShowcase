#version 330

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texture_coord;
layout (location = 2) in vec3 vertex_normal;

out vec3 vs_position;
out vec2 vs_texture_coordinate;
out vec3 vs_normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(){
    vs_position = vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
    vs_texture_coordinate = vec2(vertex_texture_coord.x, vertex_texture_coord.y * -1.f);
    vs_normal = mat3(ModelMatrix) * vertex_normal;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
}
