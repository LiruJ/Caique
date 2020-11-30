uniform mat4 modelMatrix;
uniform mat4 projection;

attribute vec3 in_Position;
attribute vec2 in_TexCoord;

varying vec2 v_TexCoord;

void main()
{
	gl_Position = projection * modelMatrix * vec4(in_Position, 1.0);
	v_TexCoord = in_TexCoord;
}