#version 430 
layout (location = 0) in vec3 VertexP;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexture;




out vec3 Normal;
out vec3 Position;
out vec2 TexCoords;

//uniform 
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

void main()
{
    Position = vec3(ModelMatrix* vec4(VertexP, 1.0));
    Normal = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;
    TexCoords = VertexTexture;

    mat4 mvp=ProjectionMatrix * ViewMatrix * ModelMatrix;
    gl_Position = mvp * vec4(VertexP,1.0);
}
