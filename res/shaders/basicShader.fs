#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;

uniform vec4 lightColor;
uniform sampler2D sampler;
uniform vec4 lightDirection;


void main()
{
    vec3 tmp = dot(-lightDirection.xyz, normal0) * color0 ;
	//gl_FragColor = texture2D(sampler, texCoord0) *clamp(vec4(tmp,1.0), 0.0, 1.0);
    gl_FragColor = vec4(0.2667f, 0.3137f, 0.6196f, 0.0f) + clamp(vec4(tmp,1),0.0,1.0);
}
