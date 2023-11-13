//
//  Mesh.cpp
//  OpenGlTest
//
//  Created by Charlie Close on 28/03/2023.
//

#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices)
{
    Mesh::vertices = vertices;
    Mesh::indices = indices;

    VAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO.setVertex(vertices);
    // Generates Element Buffer Object and links it to indices
    EBO.setIndices(indices);
    // Links VBO attributes such as coordinates and colors to VAO
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

Mesh::Mesh()
{
    Mesh::vertices = {};
    Mesh::indices = {};

    VAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO.setVertex(vertices);
    // Generates Element Buffer Object and links it to indices
    EBO.setIndices(indices);
    // Links VBO attributes such as coordinates and colors to VAO
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();    
}

void Mesh::Reset(std::vector <Vertex>& vertices, std::vector <GLuint>& indices)
{
    Mesh::vertices = vertices;
    Mesh::indices = indices;

    VAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO.setVertex(vertices);
    // Generates Element Buffer Object and links it to indices
    EBO.setIndices(indices);
    // Links VBO attributes such as coordinates and colors to VAO
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

void Mesh::Draw
(
    Shader& shader,
    Camera& camera,
    glm::vec3 offset
)
{
    // Bind shader to be able to access uniforms
    shader.Activate();
    VAO.Bind();
    
    // Take care of the camera Matrix
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    glUniform3f(glGetUniformLocation(shader.ID, "offset"), offset.x, offset.y, offset.z);
    camera.Matrix(shader, "camMatrix");

    // Draw the actual mesh
    glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
}

void Mesh::PDraw
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
)
{
    // Bind shader to be able to access uniforms
    shader.Activate();
    VAO.Bind();
    
    // Take care of the camera Matrix
    glUniform3f(glGetUniformLocation(shader.ID, "center"), cent[0], cent[1], cent[2]);
    glUniform3f(glGetUniformLocation(shader.ID, "cameraPos"), camera.Position[0], camera.Position[1], camera.Position[2]);
    glUniform1f(glGetUniformLocation(shader.ID, "radius"), rad);
    glUniform1f(glGetUniformLocation(shader.ID, "atmosRad"), atmosRad);
    glUniform3f(glGetUniformLocation(shader.ID, "atmosCol"), atmosCol[0], atmosCol[1], atmosCol[2]);
    glUniform3f(glGetUniformLocation(shader.ID, "lightDirec"), lightDirec[0], lightDirec[1], lightDirec[2]);
    glUniform1f(glGetUniformLocation(shader.ID, "atmosThick"), atmosThick);
    glUniform1f(glGetUniformLocation(shader.ID, "densFall"), densFall);
    glUniform1i(glGetUniformLocation(shader.ID, "invert"), invert);
    camera.Matrix(shader, "camMatrix");

    // Draw the actual mesh
//    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);    
}

void Mesh::Delete()
{
    VAO.Delete();
    EBO.Delete();
    VBO.Delete();
}
