#version 330 core

uniform sampler2D tex_2d;
uniform vec3 sun_position;
uniform vec3 sun_color;
uniform vec3 spotlight_position;
uniform vec3 spotlight_direction;
uniform vec3 spotlight_color;
uniform float spotlight_first_cos;
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

in vec3 Normal;
in vec3 position;
in vec2 vert_tex;

out vec4 out_col;

vec3 apply_M(vec3 in_vec) {
    vec4 out4 = m * vec4(in_vec, 1.0);
    return vec3(out4.xyz);
}

vec3 apply_V(vec3 in_vec) {
    vec4 out4 = v * vec4(in_vec, 1.0);
    return vec3(out4.xyz);
}

vec3 apply_MV(vec3 in_vec) {
    vec4 out4 = v * m * vec4(in_vec, 1.0);
    return vec3(out4.xyz);
}

void main() {
    // those are weights of different light sources
    float uncond_light_coef = 0.1, sun_light_coef = 0.6, spotlight_coef = 0.3;

    // calculate sun light
    vec3 normal = normalize(Normal);
    vec3 sunPosition = apply_M(sun_position);
    vec3 sun_direction = normalize(sunPosition - position);
    float sun_lum = max(dot(sun_direction, normal), 0.0);
    vec3 sun_light = (sun_light_coef * sun_lum) * sun_color;

    // calculate spotlight
    vec3 spotlightFallVector = normalize(spotlight_position - position);
    float spotlight_cos = dot(spotlightFallVector, normalize(-spotlight_direction));
    float spotlight_lum = float(spotlight_cos >= spotlight_first_cos);
    vec3 spotlight = (spotlight_coef * spotlight_lum) * spotlight_color;

    // calculate unconditional light
    vec3 uncond_light = uncond_light_coef * sun_color;

    // calculate the result
    out_col = texture(tex_2d, vert_tex) * vec4(sun_light + spotlight + uncond_light, 1.0);

    // for debugging purposes
    // out_col = vec4(uncond_light_coef + sun_light, 1.0);
    // out_col = vec4((spotlight_cos * 0.5) + 0.5, uncond_light_coef, uncond_light_coef, 1.0);
    // out_col = vec4(spotlight_lum, uncond_light_coef, uncond_light_coef, 1.0);
    // out_col = vec4(normal.xyz, 1.0);
}
