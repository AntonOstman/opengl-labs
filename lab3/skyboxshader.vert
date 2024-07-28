#version 150

in vec3 in_Position;
uniform mat4 modelToWorld;
uniform mat4 projection;
uniform mat4 worldToView;
in vec2 inTexCoord;
out vec2 vertTexCoord;

void main(void)
{
	gl_Position =  projection *  worldToView *  modelToWorld * vec4(in_Position, 1.0);

	vertTexCoord = inTexCoord;
}

