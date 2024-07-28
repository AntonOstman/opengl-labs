#version 150

uniform mat4 modelToView;
uniform mat4 projection;
uniform mat4 viewToWorld;

in vec3 in_Position;
in vec3 in_Normal;

out vec3 surfaceNormal;
out vec3 surfaceCoord;

void main(void)
{
    surfaceNormal = vec3(viewToWorld * modelToView * vec4(in_Normal, 0));
    surfaceCoord = vec3(viewToWorld * modelToView * vec4(in_Position, 1));
    gl_Position =  projection *  viewToWorld *  modelToView * vec4(in_Position, 1.0);
}

