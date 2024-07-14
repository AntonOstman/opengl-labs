#version 150

out vec4 out_Color;
uniform sampler2D texUnit;
in vec2 vertTexCoord;

void main(void)
{
    out_Color = texture(texUnit, vertTexCoord);
}
