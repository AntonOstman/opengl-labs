#version 150

// in vec3 vert_Normal;
out vec4 out_Color;
uniform sampler2D texUnit;
in vec2 vertTexCoord;

void main(void)
{
    out_Color = texture(texUnit, vertTexCoord);
//   out_Color = vec4(texUnit, vertTexCoord);
}
