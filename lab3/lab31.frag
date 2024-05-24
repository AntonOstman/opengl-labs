
#version 150

in vec3 normal;
out vec4 out_Color;
uniform sampler2D texUnit;
in vec2 vertTexCoord;

void main(void)
{
    // out_Color = texture(texUnit, vertTexCoord);
    const vec3 light = normalize(vec3(0.58, 0.58, 0.58));
    float strength = dot(light, normal);
    if (strength < 0){
        strength = 0;
    }

    out_Color = strength * texture(texUnit, vertTexCoord);
}
