//
//  TriNoise.cpp
//  Planets
//
//  Created by Charlie Close on 06/04/2023.


#include <stdio.h>
#include"ComplexNoise.h"
#include<glm/glm.hpp>


float PlanetNoise(float x, float y, float z)
{
    SimplexNoise Noise;
    int seed = 200;
    float Conts = fmax(fmin(Noise.noise(x / 50, y / 50, z / 50 + 10 + seed) * 0.02, 0.001), -0.005);
    float MBiome = fmin(fmax((0.2 - abs(Noise.noise(x / 60, y / 60, z / 60 + seed))), 0), 0.2) * 80 * fmax(Conts, 0);
    float Mountains = MBiome * (Noise.noise(x, y, z + 20 + seed) + 1);
    float Roughness = Noise.noise(x / 5, y / 5, z / 5 + 40 + seed) * 0.001;
    return Conts + Mountains + Roughness;
}

glm::vec3 yellow(float height, glm::vec3 norm, glm::vec3 pos)
{
    return glm::vec3(1, 1, 0);
}
glm::vec3 orange(float height, glm::vec3 norm, glm::vec3 pos)
{
    return glm::vec3(1, 0.5, 0);
}
glm::vec3 red(float height, glm::vec3 norm, glm::vec3 pos)
{
    return glm::vec3(1, 0, 0);
}

glm::vec3 PlanetColour(float height, glm::vec3 norm, glm::vec3 pos)
{
    glm::vec3 normPos = normalize(pos);
    float iceLine = (0.95 - pow(normPos.y, 2)) * 0.035;
    float angle = dot(normPos, norm);
    
    if (height > iceLine and angle > 0.7)
    {
        return glm::vec3(1, 1, 1);
    }
    if (height > 0.017)
    {
        return glm::vec3(0.4, 0.4, 0.4);
    }
    if (height > 0.005)
    {
        if (angle < 0.7)
        {
            return glm::vec3(0.4, 0.4, 0.5);
        }
        return glm::vec3(0.05, 0.4, 0.1);
    }
    return glm::vec3(0.05, 0.4, 0.1);
}

float atmosN(float x, float y, float z)
{
    return 15.0;
}

glm::vec3 atmosColl(float height, glm::vec3 norm, glm::vec3 pos)
{
    return glm::vec3(0);
}
