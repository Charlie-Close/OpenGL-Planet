//
//  TriNoise.h
//  Planets
//
//  Created by Charlie Close on 06/04/2023.
//

#ifndef TriNoise_h
#define TriNoise_h

#include"SimplexNoise.h"
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/norm.hpp>

float PlanetNoise(float x, float y, float z);
glm::vec3 PlanetColour(float height, glm::vec3 norm, glm::vec3 pos);
float atmosN(float x, float y, float z);
glm::vec3 atmosColl(float height, glm::vec3 norm, glm::vec3 pos);
glm::vec3 yellow(float height, glm::vec3 norm, glm::vec3 pos);
glm::vec3 orange(float height, glm::vec3 norm, glm::vec3 pos);
glm::vec3 red(float height, glm::vec3 norm, glm::vec3 pos);

#endif /* TriNoise_h */
