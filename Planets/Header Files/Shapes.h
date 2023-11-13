//
//  Circle.h
//  OpenGlTest
//
//  Created by Charlie Close on 25/03/2023.
//

#ifndef Circle_h
#define Circle_h

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/norm.hpp>
#include"Triangle.h"
#include"Mesh.h"


Mesh Circle(glm::vec3 position, float radius, int subDev);
Mesh Cube(glm::vec3 position, float size);

class Square
{
public:
    std::vector <Vertex> Verts;
    std::vector <int> Inds;
    std::vector <glm::vec3> Corners;
    glm::vec3 center;
    glm::vec3 avgNorm;
    float SubDivs;
    std::vector <Vertex> OgVerts;
    std::vector <Vertex> SubedVerts;
    std::vector <glm::vec3> SubedCorners;
    
    Square(glm::vec3 verts[4], float radius, float (*noise)(float x, float y, float z), glm::vec3 (*nToC)(float height, glm::vec3 norm, glm::vec3 pos));
    Square(glm::vec3 verts[4], float radius, glm::vec3 colour);
    Square(Vertex verts[5], glm::vec3 corners[4]);
    Square(std::vector <Vertex> verts, std::vector <glm::vec3> corners);
    
    void SubDivideSq(int subs, Square& square, float radius, float (*noise)(float x, float y, float z), glm::vec3 (*nToC)(float height, glm::vec3 norm, glm::vec3 pos));
};

std::vector <Square> SubDivide(Square& square, float radius, float (*noise)(float x, float y, float z), glm::vec3 (*nToC)(float height, glm::vec3 norm, glm::vec3 pos));

std::vector <Square> SubDivide(Square square, float radius, glm::vec3 colour);


#endif /* Circle_h */
