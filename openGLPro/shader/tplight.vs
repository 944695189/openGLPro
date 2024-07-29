#version 430 //版本号
//输入
layout (location = 0) in vec3 VertexP;//指名第一个数据作为顶点位置属性（变量名称自定义）
layout (location = 1) in vec2 VertexTexture;//指名第二个数据为纹理坐标属性
layout (location = 2) in vec3 VertexNormal;//指名第三个为法线属性

//layout 指定变量在顶点数据中的位置索引。in 关键字声明输入变量，表示变量是从外部传递给顶点着色器的输入数据。

//输出
out vec3 Normal;
out vec3 Position;
out vec2 TexCoords;

//uniform 变量
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

void main()
{
    Position = vec3(ModelMatrix* vec4(VertexP, 1.0));
    Normal = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;//计算模型变换后的法向
    TexCoords = VertexTexture;

    mat4 mvp=ProjectionMatrix * ViewMatrix * ModelMatrix;
    gl_Position = mvp * vec4(VertexP,1.0);
}
