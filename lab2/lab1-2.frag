#version 150

// in vec3 vert_Normal;
out vec4 out_Color;
in vec2 vertTexCoord;

void main(void)
{
  float a = sin(vertTexCoord[1]*30)/2;
  float b = sin(vertTexCoord[0]*30)/2;

  //float a = vertTexCoord.s;
  //float b = vertTexCoord.t;

//   vec3 vert_Normalized = normalize(vert_Normal);
  out_Color = vec4(a, b, 0, 1.0);

}
