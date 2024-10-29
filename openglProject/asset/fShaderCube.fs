#version 430

in vec3 tc;
in float altitude;
out vec4 fragColor;

uniform mat4 v_matrix;
uniform mat4 p_matrix;
uniform int isAbove;
layout (binding = 0) uniform samplerCube samp;

void main(void)
{	
	 fragColor = texture(samp,tc);
}
