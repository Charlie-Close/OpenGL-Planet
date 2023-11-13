//
//  Player.cpp
//  Planets
//
//  Created by Charlie Close on 24/04/2023.
//

#include"Player.h"

Player::Player(glm::vec3 position, glm::vec3 rotation)
{
    Model model("ObjFiles/astronaut.obj", 0, glm::vec3(1, 0, 0), 0.002, glm::vec3(0, -0.002, 0.000));
//    Model model("StlFiles/astronaut.stl", 3.1415 / 2, glm::vec3(1, 0, 0), 0.002, glm::vec3(-0.00112, -0.002, 0.0005), glm::vec3(1, 1, 1));
    
    mesh = model.ModelMesh;
    pos = position;
    rot = rotation;
    
    for (int i = 0; i < mesh.vertices.size(); i++)
    {
        if (mesh.vertices[i].position[1] < -0.0010) {
            if (mesh.vertices[i].position[0] > 0) {
                lshin.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.01;
            } else {
                rshin.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.02;
            }
        } else if (mesh.vertices[i].position[1] < -0.0008) {
            if (mesh.vertices[i].position[0] > 0) {
                lknee.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.03;
            } else {
                rknee.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.04;
            }
        } else if (mesh.vertices[i].position[2] > -0.0003 and abs(mesh.vertices[i].position[0]) < 0.0006 and (mesh.vertices[i].position[1] < -0.0002)) {
            if (mesh.vertices[i].position[0] > 0) {
                lthi.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.05;
            } else {
                rthi.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.06;
            }
        } else if (mesh.vertices[i].position[2] > -0.0003 and abs(mesh.vertices[i].position[0]) < 0.0006 and ((mesh.vertices[i].position[1] < 0 and abs(mesh.vertices[i].position[0]) > 0.0003))) {
            if (mesh.vertices[i].position[0] > 0) {
                lhip.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.07;
            } else {
                rhip.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.08;
            }
        } else if (abs(mesh.vertices[i].position[0]) > 0.0007 and mesh.vertices[i].position[1] < 0.0003) {
            if (mesh.vertices[i].position[0] > 0) {
                lfor.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.09;
            } else {
                rfor.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.10;
            }
        } else if (abs(mesh.vertices[i].position[0]) > 0.0007 and mesh.vertices[i].position[1] < 0.0005) {
            if (mesh.vertices[i].position[0] > 0) {
                lelb.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.11;
            } else {
                relb.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.12;
            }
        } else if (mesh.vertices[i].position[2] > -0.0004 and abs(mesh.vertices[i].position[0]) > 0.0005 and mesh.vertices[i].position[1] < 0.0008 and mesh.vertices[i].position[1] > 0.0005 ) {
            if (mesh.vertices[i].position[0] > 0) {
                rbic.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.13;
            } else {
                lbic.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.14;
            }
        } else if ((mesh.vertices[i].position[2] > -0.0001 or floor(mesh.vertices[i].position[2] * 1000000) == -132) and abs(mesh.vertices[i].position[0]) > 0.0005 and mesh.vertices[i].position[1] < 0.0012 and mesh.vertices[i].position[1] > 0.0008 ) {
            if (mesh.vertices[i].position[0] > 0) {
                rsho.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.15;
            } else {
                lsho.push_back(mesh.vertices[i]);
                mesh.vertices[i].colour[2] = 0.16;
            }
        } else {
            tors.push_back(mesh.vertices[i]);
        }
    }
        
    mesh = Mesh(mesh.vertices, mesh.indices);
}


void Player::Draw(Camera &camera, Shader &playerProgram, Texture tex){
    tex.Bind();
    glm::mat4 lkneem = glm::rotate(glm::mat4(1), lkneet, glm::vec3(1, 0, 0));
    glm::mat4 rkneem = glm::rotate(glm::mat4(1), rkneet, glm::vec3(1, 0, 0));
    glm::mat4 lhipm = glm::rotate(glm::mat4(1), lhipt, glm::vec3(1, 0, 0));
    lhipm = glm::rotate(lhipm, lhipp, glm::vec3(0, 0, 1));
    glm::mat4 rhipm = glm::rotate(glm::mat4(1), rhipt, glm::vec3(1, 0, 0));
    rhipm = glm::rotate(rhipm, rhipp, glm::vec3(0, 0, 1));
    glm::mat4 lshom = glm::rotate(glm::mat4(1), lshop, glm::vec3(0, 0, 1));
    lshom = glm::rotate(lshom, lshot, glm::vec3(1, 0, 0));
    glm::mat4 rshom = glm::rotate(glm::mat4(1), rshop, glm::vec3(0, 0, 1));
    rshom = glm::rotate(rshom, lshot, glm::vec3(1, 0, 0));
    glm::mat4 lelbm = glm::rotate(glm::mat4(1), lelbt, glm::vec3(1, 0, 0));
    glm::mat4 relbm = glm::rotate(glm::mat4(1), relbt, glm::vec3(1, 0, 0));
    
    if (lhipt < 1) {
        lhipt += 0.001;
        lkneet -= 0.001;
    }
//    lhipp -= 0.001;
    glUseProgram(playerProgram.ID);
    
    glUniformMatrix4fv(glGetUniformLocation(playerProgram.ID, "lknee"), 1, GL_FALSE, &lkneem[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(playerProgram.ID, "rknee"), 1, GL_FALSE, &rkneem[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(playerProgram.ID, "lhip"), 1, GL_FALSE, &lhipm[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(playerProgram.ID, "rhip"), 1, GL_FALSE, &rhipm[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(playerProgram.ID, "lsho"), 1, GL_FALSE, &lshom[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(playerProgram.ID, "rsho"), 1, GL_FALSE, &rshom[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(playerProgram.ID, "lelb"), 1, GL_FALSE, &lelbm[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(playerProgram.ID, "relb"), 1, GL_FALSE, &relbm[0][0]);
    
    mesh.Draw(playerProgram, camera, pos);
    tex.Unbind();
}

