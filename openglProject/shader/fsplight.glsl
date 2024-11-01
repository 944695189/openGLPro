#version 430

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 Ka; // 环境反射系数
uniform vec3 Kd; //漫反射系数
uniform vec3 Ks; // 镜面反射系数
uniform int shiness;//镜面反射的光泽度

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
layout(binding=0) uniform sampler2D text;//设置2d纹理采样器text

void main() 
{
    // phong shading模型
    // 环境光
    vec3 ambient = Ka*lightColor * vec3(texture(text, TexCoords));
  	
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = Kd*lightColor * diff * vec3(texture(text, TexCoords));
    
    //  镜面反射
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
    vec3 specular =Ks*lightColor * spec * vec3(texture(text, TexCoords));
    vec3 Color = ambient +  diffuse + specular;   

    FragColor = vec4(Color, 1.0);//将 RGB 颜色值和 alpha（透明度）值设为 1.0，表示完全不透明
}
