#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 col;
layout(location=2) in vec2 tex;

uniform mat4 mvp;

out vec2 vert_tex;

void main() {
        vert_tex = tex;
        gl_Position = mvp * vec4(pos.xyz, 1.0);
}
