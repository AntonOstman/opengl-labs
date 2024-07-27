#version 150


in vec3 in_Position;
in vec3 camPos;
out vec3 camVec;
uniform mat4 scaleMatrix;
uniform mat4 projection;

in vec3 in_Normal;
out vec4 viewNormal;
out vec3 normal;
uniform mat4 lookAt;

void main(void)
{
    mat4 model = scaleMatrix;
    normal = mat3(model) * in_Normal;
    gl_Position =  projection *  lookAt *  model * vec4(in_Position, 1.0);
    vec3 pos = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
    camVec = normalize(pos - camPos);
    viewNormal = normalize(lookAt * model * vec4(in_Normal, 0.0));
}

