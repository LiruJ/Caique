uniform sampler2D u_Texture0;

uniform vec4 u_Colour0;

varying vec2 v_UVs;

void main()
{
	vec4 texel = texture2D(u_Texture0, v_UVs);

	gl_FragColor = texel * u_Colour0;

}