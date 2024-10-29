#version 430

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 Ka; // Ambient reflectivity
uniform vec3 Kd; // Diffuse reflectivity
uniform vec3 Ks; // Specular reflectivity
uniform int shiness;//�߹�ϵ��

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
layout(binding=0) uniform sampler2D text;

void main() 
{
    // phong shading
    // ambient
    vec3 ambient = Ka*lightColor * vec3(texture(text, TexCoords));//Ka
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = Kd*lightColor * diff * vec3(texture(text, TexCoords));// Kd 
    
    // specular
    //  float specularStrength = 1.0; 
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
    vec3 specular =Ks*lightColor * spec * vec3(texture(text, TexCoords));      //Ks
    vec3 Color = ambient +  diffuse + specular;//    
    // phong shading end

    FragColor = vec4(Color, 1.0);
}
