uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

attribute vec3 in_Position;
attribute vec2 in_UVs;

varying vec2 v_UVs;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(in_Position, 1.0);
	v_UVs = in_UVs;
}