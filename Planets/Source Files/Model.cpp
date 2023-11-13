//
//  Model.cpp
//  OpenGlTest
//
//  Created by Charlie Close on 28/03/2023.
//

#include"Model.h"

void maxmin(float* max, float* min, float val)
{
    if (val < *min) {
        *min = val;
    }
    if (*max < val) {
        *max = val;
    }
}

Model::Model(std::string filepath, float rotSize, glm::vec3 rotVec, float scale, glm::vec3 position, glm::vec3 colour)
{
    glm::mat4 transform = glm::rotate(glm::mat4(scale), rotSize, rotVec);

    stl_reader::StlMesh <float, unsigned int> mesh (filepath);
    
    try {
        for(size_t itri = 0; itri < mesh.num_tris(); ++itri) {
            const float* c0 = mesh.tri_corner_coords (itri, 0);
            const float* c1 = mesh.tri_corner_coords (itri, 1);
            const float* c2 = mesh.tri_corner_coords (itri, 2);

            glm::vec3 v0(c0[0], c0[1], c0[2]);
            glm::vec3 v1(c1[0], c1[1], c1[2]);
            glm::vec3 v2(c2[0], c2[1], c2[2]);

            const float* n = mesh.tri_normal (itri);
            glm::vec3 norm = glm::vec3(n[0], n[1], n[2]);

            verts.push_back(Vertex{glm::vec3(glm::vec4(c0[0], c0[1], c0[2], 1) * transform) + position, norm, colour});
            verts.push_back(Vertex{glm::vec3(glm::vec4(c1[0], c1[1], c1[2], 1) * transform) + position, norm, colour});
            verts.push_back(Vertex{glm::vec3(glm::vec4(c2[0], c2[1], c2[2], 1) * transform) + position, norm, colour});

            ind.push_back(GLuint(itri * 3));
            ind.push_back(GLuint(itri * 3 + 1));
            ind.push_back(GLuint(itri * 3 + 2));
        }
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
    std::cout << verts.size() << std::endl;
    
    ModelMesh = Mesh(verts, ind);
}

Model::Model(std::string filepath, float rotSize, glm::vec3 rotVec, float scale, glm::vec3 position)
{
    verts = LoadOBJ(filepath);
    glm::mat4 transform = glm::rotate(glm::mat4(scale), rotSize, rotVec);
    
    for (int i = 0; i < verts.size(); i ++) {
        verts[i].position = glm::vec3(glm::vec4(verts[i].position, 1) * transform) + position;
        ind.push_back(i);
    }
    
    std::cout << verts.size() << std::endl;
    
    ModelMesh = Mesh(verts, ind);
}
