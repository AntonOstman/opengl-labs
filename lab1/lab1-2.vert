#version 150

in  vec3 in_Position;
uniform mat4 scaleMatrix;
in vec3 in_Normal;
out vec3 vert_Normal;
in vec2 inTexCoord;
out vec2 vertTexCoord;

void main(void)
{
	gl_Position = scaleMatrix*vec4(in_Position, 1.0);
	//vert_Color = in_Color;
	vert_Normal = in_Normal;
	vertTexCoord = inTexCoord;
}
