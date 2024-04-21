#version 150

in vec3 in_Position;
uniform mat4 scaleMatrix;
in vec2 inTexCoord;
out vec2 vertTexCoord;

void main(void)
{
	gl_Position = scaleMatrix*vec4(in_Position, 1.0);
	vertTexCoord = inTexCoord;
}

