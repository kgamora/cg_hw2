#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec2 tex;

uniform mat4 mvp;
uniform mat4 m;

out vec3 normal;
out vec3 position;
out vec2 vert_tex;

void main() {
    vert_tex = tex;
    normal = (m * vec4(in_normal, 1.0)).xyz;
    normal = normalize(normal);
    gl_Position = mvp * vec4(pos, 1.0);
    position = gl_Position.xyz;
}
