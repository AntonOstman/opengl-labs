#version 150

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform int isDirectional[4];
uniform vec3 ambientColor;
uniform float specularExponent;
uniform vec3 camPos;
uniform mat4 worldToView;

in vec3 viewNormal;
in vec3 worldNormal;
in vec3 surfaceCoord;

out vec4 out_Color;
void main(void)
{
    // vec3 backgroundLight = vec3(1, 1, 1);
    //vec3 ambientLight = ambientColor * clamp(dot(backgroundLight, worldNormal),0,1) * 0.5;
    vec3 totalLight = vec3(0, 0, 0);
    totalLight += ambientColor*0.05;

    vec3 cameraDirection = normalize(vec3(worldToView * vec4(surfaceCoord, 1)));

    for (int i = 0; i < 4; i++){

        int isPositional = (1 - isDirectional[i]);
        vec3 lightIncident = normalize(mat3(worldToView) * (lightSourcesDirPosArr[i] - surfaceCoord * isPositional));

        vec3 lightColor = lightSourcesColorArr[i];
        float strength = max(0, dot(lightIncident, normalize(viewNormal)));
        vec3 diffuseLight = strength * lightColor;
        totalLight += diffuseLight * 0.5;

        vec3 reflection = normalize(reflect(lightIncident, normalize(viewNormal)));
        if (strength > 0.001){
            float specular = pow(max(dot(reflection, cameraDirection), 0.01), specularExponent);
            totalLight += specular * lightColor * 0.8;
        }
    }


    out_Color = vec4(totalLight, 1);
    // out_Color = vec4(surfaceCoord,1);
    // out_Color = vec4(viewNormal, 1);

}
