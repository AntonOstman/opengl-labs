
#version 150

in vec3 normal;
out vec4 out_Color;

void main(void)
{
    const vec3 light = normalize(vec3(0.58, 0.58, 0.58));
    float strength = dot(light, normal);

    strength = max(strength, 0);

    out_Color = strength * vec4(1.0);
}
