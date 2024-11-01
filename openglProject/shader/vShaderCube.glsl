#version 430

layout (location = 0) in vec3 position;
out vec3 tc;
out float altitude;

uniform mat4 v_matrix;
uniform mat4 p_matrix;
uniform int isAbove;
layout (binding = 0) uniform samplerCube samp;//设置采样器samp

void main(void)
{
	tc = position;
	mat4 v3_matrix = mat4(mat3(v_matrix));//删除平移分量仅保留旋转然后 3x3 矩阵转换回 4x4 矩阵，实现天空盒跟随相机的效果，模拟无限远的立方体环境
	gl_Position = p_matrix * v3_matrix * vec4(position,1.0);
	altitude = gl_Position.y;//计算高度
}
