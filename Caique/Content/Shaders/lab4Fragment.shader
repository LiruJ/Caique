uniform sampler2D u_Texture;

varying vec2 v_TexCoord;

void main()
{
	vec4 texel = texture2D(u_Texture, v_TexCoord);
	gl_FragColor = texel;
}