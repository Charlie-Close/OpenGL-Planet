//
//  Planet.h
//  Planets
//
//  Created by Charlie Close on 05/04/2023.
//

#ifndef Planet_h
#define Planet_h

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/norm.hpp>
#include<thread>
#include"Texture.h"

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"shaderClass.h"
#include"Shapes.h"
#include"ComplexNoise.h"

class Planet
{
public:
    glm::vec3 position;
    float radius;
    float atmoRad;
    std::vector <Square> squares;
    std::vector <Square> setSquares;
    std::vector <Vertex> verts;
    std::vector <GLuint> inds;
    Mesh mesh;
    Mesh loaded;
    Mesh water;
    Mesh atmos;
    float (*noise)(float x, float y, float z);
    glm::vec3 (*PColour)(float height, glm::vec3 norm, glm::vec3 pos);
    
    Planet(glm::vec3 pos, float rad);
    
    void CalculateMesh(glm::vec3 pos);
    void SetMesh();
    
    std::vector <Square> SubDividedN(Square square, int depth, int goalDepth, glm::vec3 colour, float rad);
    std::vector <Square> SubDivided(Square square, int depth, int goalDepth);
    std::vector <Square> SubDivided(Square& square, int depth, glm::vec3 pos);
    
    void Draw(Camera &camera, Shader &atmosProgram, Shader &planetProgram, Shader &waterProgram, GLuint renderedTexture, Texture grass);
    void DrawR(Camera &camera, Shader &atmosProgram, Shader &planetProgram, Texture grass);
    void SetShaderData(Camera &camera, Shader &shader);
};

#endif /* Planet_h */
