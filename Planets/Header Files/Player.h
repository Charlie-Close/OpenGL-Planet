//
//  Player.h
//  Planets
//
//  Created by Charlie Close on 24/04/2023.
//

#ifndef Player_h
#define Player_h

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/norm.hpp>
#include<vector>
#include"Texture.h"

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"shaderClass.h"
#include"Model.h"
#include"Mesh.h"
#include"Planet.h"
#include"obj_reader.h"
#include"Texture.h"


class Player
{
public:
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 vel;
    Mesh mesh;

    std::vector<Vertex> lshin;
    std::vector<Vertex> lknee;
    float lkneet;
    std::vector<Vertex> lthi;
    std::vector<Vertex> lhip;
    float lhipt;
    float lhipp;
    std::vector<Vertex> lsho;
    float lshot;
    float lshop;
    std::vector<Vertex> lbic;
    std::vector<Vertex> lelb;
    float lelbt;
    std::vector<Vertex> lfor;
    std::vector<Vertex> rshin;
    std::vector<Vertex> rknee;
    float rkneet;
    std::vector<Vertex> rthi;
    std::vector<Vertex> rhip;
    float rhipt;
    float rhipp;
    std::vector<Vertex> rsho;
    float rshot;
    float rshop;
    std::vector<Vertex> rbic;
    std::vector<Vertex> relb;
    float relbt;
    std::vector<Vertex> rfor;
    std::vector<Vertex> tors;
    
    Player(glm::vec3 position, glm::vec3 rotation);
    
    void Draw(Camera &camera, Shader &playerProgram, Texture tex);
    void updateVals(Planet local);
};


#endif /* Player_h */
