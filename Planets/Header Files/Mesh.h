//
//  Mesh.h
//  OpenGlTest
//
//  Created by Charlie Close on 28/03/2023.
//

#ifndef Mesh_h
#define Mesh_h

#include<string>
#include<vector>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"

class Mesh
{
public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    // Store VAO in public so it can be used in the Draw function
    VAO VAO;
    VBO VBO;
    EBO EBO;

    // Initializes the mesh
    Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);
    Mesh();
    void Reset(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);
    void Delete();

    // Draws the mesh
    void Draw
    (
        Shader& shader,
        Camera& camera,
        glm::vec3 offset
    );
    
    void PDraw
    (
        Shader& shader,
        Camera& camera,
        glm::vec3 cent,
        float rad,
        float atmosRad,
        float atmosThick,
        glm::vec3 atmosCol,
        float densFall,
        glm::vec3 lightDirec,
        bool invert
     );
};

#endif /* Mesh_h */
