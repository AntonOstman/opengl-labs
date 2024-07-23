#version 150

in vec3 in_Position;
uniform mat4 scaleMatrix;
uniform mat4 projection;
uniform mat4 lookAt;
in vec2 inTexCoord;
out vec2 vertTexCoord;

void main(void)
{

    mat4 world = scaleMatrix;
	gl_Position =  projection *  lookAt *  world * vec4(in_Position, 1.0);

	vertTexCoord = inTexCoord;
}

