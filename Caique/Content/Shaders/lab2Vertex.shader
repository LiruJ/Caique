attribute vec3 in_Position;
attribute vec4 in_Colour;

varying vec4 v_Colour;

void main()
{
	gl_Position = vec4(in_Position, 1.0);
	v_Colour = in_Colour;
}