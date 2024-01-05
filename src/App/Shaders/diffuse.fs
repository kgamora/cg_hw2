#version 330 core

uniform sampler2D tex_2d;
uniform vec3 sun_position;
uniform vec3 sun_color;
uniform vec3 spotlight_position;
uniform vec3 spotlight_direction;
uniform vec3 spotlight_color;
uniform float spotlight_first_cos;
uniform float spotlight_second_cos;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

in vec3 Normal;
in vec3 position;
in vec2 vert_tex;

out vec4 out_col;

void main() {
    // those are weights of different light sources
    float ambient_light_coef = 0.1, sun_light_coef = 0.6, spotlight_coef = 0.3;

    vec3 normal = normalize(Normal);

    // calculate sun light
    vec3 sunPosition = vec3(m * vec4(sun_position, 1.0));
    vec3 sun_direction = normalize(sunPosition - position);
    float sun_lum = max(dot(sun_direction, normal), 0.0);
    vec3 sun_light = (sun_light_coef * sun_lum) * sun_color;

    // calculate spotlight
    vec3 spotlightFallVector = normalize(spotlight_position - position);
    float spotlight_cos = dot(spotlightFallVector, normalize(-spotlight_direction));
    float spotlight_lum = float(spotlight_cos >= spotlight_first_cos);
    spotlight_lum += float(spotlight_cos < spotlight_first_cos)
                        * float(spotlight_cos >= spotlight_second_cos)
                        * abs(spotlight_cos - spotlight_second_cos) / (spotlight_first_cos - spotlight_second_cos);
    vec3 spotlight = (spotlight_coef * spotlight_lum) * spotlight_color;

    // calculate ambient light
    vec3 ambient_light = ambient_light_coef * sun_color;

    // calculate the result
    out_col = texture(tex_2d, vert_tex) * vec4(sun_light + spotlight + ambient_light, 1.0);
}
