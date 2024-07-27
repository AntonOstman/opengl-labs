#version 150

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform bool isDirectional[4];
uniform vec3 ambientColor;
uniform float specularExponent;

in vec3 normal;
in vec3 camVec;
out vec4 out_Color;
in vec4 viewNormal;
// const vec3 light = normalize(vec3(-78.29903, 86.01693, 10.91323));
// const vec3 light = normalize(vec3(0.58, 0.58, 0.58));
void main(void)
{
    vec3 lightPos = lightSourcesDirPosArr[1];
    vec3 lightNormal = lightSourcesDirPosArr[2];
    vec3 vec3viewNormal = normalize(vec3(viewNormal.x, viewNormal.y, viewNormal.z));
    vec3 lightIncident = normalize(camVec - lightPos);
    vec3 lightColor = lightSourcesColorArr[2];

    float strength = dot(lightIncident, normalize(normal));
    vec3 diffuseLight = strength * lightColor;

    vec3 lightR = normalize(reflect(lightIncident, vec3viewNormal));
    float specular = pow(dot(lightR, vec3viewNormal), specularExponent);
    vec3 specularLight = specular * lightColor;
    // out_Color = vec4(camVec*255, 1);
    // out_Color = normalize(viewNormal);
    // out_Color = vec4(ambientColor * 0.5 + 2*lightColor*strength, 1);
    // out_Color = vec4(ambientColor ,1);
    // out_Color = vec4(lightColor*(0.2 + strength + specular), 1);
    out_Color = vec4(ambientColor*0.2 + diffuseLight + specularLight, 1);

}
