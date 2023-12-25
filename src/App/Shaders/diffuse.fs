#version 330 core

uniform sampler2D tex_2d;
uniform vec3 sun_position;
uniform vec3 sun_color;

in vec3 normal;
in vec3 position;
in vec2 vert_tex;

out vec4 out_col;

void main() {
    float lum = max(dot(normal, normalize(sun_position)), 0.0);
    out_col = texture(tex_2d, vert_tex) * vec4((0.3 + 0.7 * lum) * sun_color, 1.0);
}
