#version 330 core

uniform sampler2D tex_2d;
uniform vec3 sun_position;
uniform vec3 sun_color;
uniform mat4 mvp;

in vec3 normal;
in vec3 position;
in vec2 vert_tex;

out vec4 out_col;

void main() {
    vec3 sunPosition = (mvp * vec4(sun_position, 1.0)).xyz;
    vec3 sun_direction = normalize(sunPosition - position);
    float lum = max(abs(dot(sun_direction, normal)), 0.0);
    out_col = texture(tex_2d, vert_tex) * vec4((0.3 + 0.7 * lum) * sun_color, 1.0);

    // for debugging purposes
    // out_col = vec4((0.3 + 0.7 * lum) * sun_color, 1.0);
    // out_col = vec4(normal.xyz, 1.0);
}
