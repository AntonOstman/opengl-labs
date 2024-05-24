
#version 150

in vec3 in_Position;
uniform mat4 scaleMatrix;
uniform mat4 projection;
uniform mat4 rotTrans;
in vec3 in_Normal;
out vec3 normal;
uniform mat4 lookAt;
in vec2 inTexCoord;
out vec2 vertTexCoord;

void main(void)
{

    mat4 world =  rotTrans * scaleMatrix;
    normal = mat3(world) * in_Normal;
	gl_Position =  projection *  lookAt *  world * vec4(in_Position, 1.0);


	vertTexCoord = inTexCoord;
}

