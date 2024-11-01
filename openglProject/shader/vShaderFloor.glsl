#version 430

// 输入顶点属性位置、纹理坐标和法线
layout (location=0) in vec3 position;   
layout (location=1) in vec2 texCoord; 
layout (location=2) in vec3 vertNormal; 

// 输出变量，传递到片元着色器
out vec3 varyingNormal; 
out vec3 varyingLightDir; 
out vec3 varyingVertPos;
out vec2 tc; 

// 3D 噪声纹理采样器，用于在片元着色器中生成噪声效果
layout (binding=0) uniform sampler3D noiseTex;

// 定义光源结构体
struct PositionalLight
{
    vec4 ambient;   
    vec4 diffuse;  
    vec4 specular; 
    vec3 position;  
};

// 定义材质结构体
struct Material
{
    vec4 ambient;   
    vec4 diffuse;  
    vec4 specular;   
    float shininess; 
};

// Uniform 变量
uniform vec4 globalAmbient;  
uniform PositionalLight light;
uniform Material material; 
uniform mat4 mv_matrix;       
uniform mat4 proj_matrix;   
uniform mat4 norm_matrix;  
uniform int isAbove;   
uniform float depthOffset;  
void main(void)
{
    // 将顶点从模型空间变换到相机空间，并传递给 varyingVertPos
    varyingVertPos = (mv_matrix * vec4(position,1.0)).xyz;

    // 计算光源方向，并传递到 varyingLightDir
    varyingLightDir = light.position - varyingVertPos;

    // 将法线从模型空间变换到相机空间，并传递给 varyingNormal
    varyingNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz;

    // 计算顶点的最终位置，将其从模型空间变换到裁剪空间
    gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);

    // 传递纹理坐标到片元着色器
    tc = texCoord;
} 
