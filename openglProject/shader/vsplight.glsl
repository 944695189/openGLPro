#version 430


layout (location = 0) in vec3 VertexP;//顶点位置数据
layout (location = 1) in vec3 VertexNormal;//法线数据
layout (location = 2) in vec2 VertexTexture;//纹理坐标

out vec3 Normal;// 输出到片元着色器的法线
out vec3 Position;//输出到片元着色器的转换后的坐标位置
out vec2 TexCoords;//输出片元着色器纹理坐标

uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

void main()
{
    Position = vec3(ModelMatrix* vec4(VertexP, 1.0));//VertexP 转换到世界空间
    Normal = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;//计算法线在变换后的空间中的方向
    TexCoords = VertexTexture;//纹理坐标 VertexTexture 直接传递给片元着色器

    mat4 mvp=ProjectionMatrix * ViewMatrix * ModelMatrix;// 计算MVP 矩阵
//首先应用 ModelMatrix，将顶点从模型空间转换到世界空间。
//然后应用 ViewMatrix，将世界空间的坐标转换为视图空间。
//最后应用 ProjectionMatrix，将视图空间的坐标转换为裁剪空间。
    gl_Position = mvp * vec4(VertexP,1.0);
}
