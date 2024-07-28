#version 150

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform bool isDirectional[4];
uniform vec3 ambientColor;
uniform float specularExponent;
uniform vec3 camPos;

in vec3 surfaceNormal;
in vec3 surfaceCoord;

out vec4 out_Color;
void main(void)
{
    vec3 lightPos = lightSourcesDirPosArr[1];
    vec3 lightIncident = normalize(camPos - lightPos);
    vec3 lightColor = lightSourcesColorArr[2];

    float strength = dot(lightIncident, normalize(surfaceNormal));
    vec3 diffuseLight = strength * lightColor;

    vec3 lightR = normalize(reflect(lightIncident, camPos - surfaceCoord));
    float specular = pow(dot(lightR, surfaceNormal), specularExponent);
    vec3 specularLight = specular * lightColor;
    out_Color = vec4(ambientColor + diffuseLight + specularLight, 1);
    // out_Color = vec4(surfaceCoord,1);
    // out_Color = vec4(worldNormal, 1);

}
