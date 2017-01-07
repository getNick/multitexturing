precision mediump float;
varying vec2 v_uv;
uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_mask;

void main()
{
	float scale=20.0;
	vec2 scaled_uv=v_uv*scale;
	vec4 tex0=texture2D(u_texture_0,scaled_uv);
	vec4 tex1=texture2D(u_texture_1,scaled_uv);
	vec4 tex2=texture2D(u_texture_2,scaled_uv);
	vec4 mask=texture2D(u_texture_mask,v_uv);
	gl_FragColor =(mask.r*tex1+mask.b*tex2+mask.g*tex0)/(mask.r+mask.b+mask.g);
}
