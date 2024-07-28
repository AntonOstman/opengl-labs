#version 150

uniform mat4 modelToWorld;
uniform mat4 projection;
uniform mat4 worldToView;

in vec3 in_Position;
in vec3 in_Normal;

out vec3 worldNormal;
out vec3 viewNormal;
out vec3 surfaceCoord;

void main(void)
{
    worldNormal = vec3(modelToWorld * vec4(in_Normal, 0));
    viewNormal = vec3(worldToView * modelToWorld * vec4(in_Normal, 0));
    surfaceCoord = vec3(modelToWorld * vec4(in_Position, 1));
    gl_Position =  projection *  worldToView *  modelToWorld * vec4(in_Position, 1.0);
}

