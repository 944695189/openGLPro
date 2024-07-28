#pragma once

#include "Shader.h"

#define MAX_BONE_INFLUENCE 4
struct Vertex {//定义顶点变量
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};
struct Texture {//定义纹理变量
    unsigned int id;
    string type;
    string path;
};
class Mesh {
public:
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;
    /*  函数  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader shader);
private:
    /*  渲染数据  */
    unsigned int VBO, EBO;
    /*  函数  */
    void setupMesh();
};