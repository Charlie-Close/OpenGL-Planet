//
//  Model.h
//  OpenGlTest
//
//  Created by Charlie Close on 28/03/2023.
//

#ifndef Model_h
#define Model_h

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"Mesh.h"
#include"stl_reader.h"
#include"Triangle.h"
#include"obj_reader.h"
#include<string>

class Model
{
public:
    std::vector <Vertex> verts;
    std::vector <GLuint> ind;

    Mesh ModelMesh;
    
    Model(std::string filepath, float rotSize, glm::vec3 rotVec, float scale, glm::vec3 position, glm::vec3 colour);
    Model(std::string filepath, float rotSize, glm::vec3 rotVec, float scale, glm::vec3 position);
};


#endif /* Model_h */
