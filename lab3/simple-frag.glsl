#version 150

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform bool isDirectional[4];
uniform float specularExponent;

in vec3 normal;
in vec3 camVec;
out vec4 out_Color;
in vec4 viewNormal;
// const vec3 light = normalize(vec3(-78.29903, 86.01693, 10.91323));
void main(void)
{
    const vec3 light = normalize(vec3(0.58, 0.58, 0.58));
    float strength = dot(light, normal);
    float ambient = 0.2;
    strength = max(strength, ambient);
    // reflect(camVec, viewNormal);
    // out_Color = vec4(camVec*255, 1);
    out_Color = strength * normalize(viewNormal);
}
