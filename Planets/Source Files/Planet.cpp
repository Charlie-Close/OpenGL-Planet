//
//  Planet.cpp
//  Planets
//
//  Created by Charlie Close on 05/04/2023.
//

#include <stdio.h>
#include "Planet.h"

std::vector <Square> SubDivided(Square square, int depth, int goalDepth, float radius, float (*noise)(float x, float y, float z), glm::vec3 (*PColour)(float height, glm::vec3 norm, glm::vec3 pos))
{
    std::vector <Square> ToRet;
    if (goalDepth <= depth)
    {
        ToRet.push_back(square);
    } else {
        std::vector <Square> New = SubDivide(square, radius, noise, PColour);
        for (int i = 0; i < New.size(); i++)
        {
            std::vector <Square> ToAdd = SubDivided(New[i], depth + 1, goalDepth, radius, noise, PColour);
            ToRet.insert(ToRet.end(), ToAdd.begin(), ToAdd.end());
        }
    }
    return ToRet;
}

void genSquares(Square Sq, int depth, std::vector <Square>& setSquares, float radius, float (*noise)(float x, float y, float z), glm::vec3 (*PColour)(float height, glm::vec3 norm, glm::vec3 pos))
{
    std::vector <Square> temp = SubDivided(Sq, 0, depth, radius, noise, PColour);
    for (int k = 0; k < temp.size(); k++)
    {
        setSquares.push_back(temp[k]);
    }
}

void SubDivided(Square& square, int depth, glm::vec3 pos, glm::vec3 position, float radius, float (*noise)(float x, float y, float z), glm::vec3 (*PColour)(float height, glm::vec3 norm, glm::vec3 pos), std::vector <Square>& setSquares, int goalDep)
{
    if (goalDep <= depth)
    {
        setSquares.push_back(square);
    } else {
        std::vector <Square> New = SubDivide(square, radius, noise, PColour);
        for (int i = 0; i < New.size(); i++)
        {
            SubDivided(New[i], depth + 1, pos, position, radius, noise, PColour, setSquares, goalDep);
        }
    }
}

void SubDividedAsy(std::vector <Square>& squares, glm::vec3 pos, glm::vec3 position, float radius, float (*noise)(float x, float y, float z), glm::vec3 (*PColour)(float height, glm::vec3 norm, glm::vec3 pos), std::vector <Square>& setSquares, int goalDepth)
{
    for (int i = 0; i < squares.size(); i++)
    {
        SubDivided(squares[i], 0, pos, position, radius, noise, PColour, setSquares, goalDepth);
    }
}

Planet::Planet(glm::vec3 pos, float rad)
{
    position = pos;
    radius = rad;
    atmoRad = radius + 50;
        
    glm::vec3 Cs[8] =
    {
        glm::vec3(-1, -1, -1),
        glm::vec3(1, -1, -1),
        glm::vec3(-1, 1, -1),
        glm::vec3(1, 1, -1),
        glm::vec3(-1, -1, 1),
        glm::vec3(1, -1, 1),
        glm::vec3(-1, 1, 1),
        glm::vec3(1, 1, 1)
    };
    
    glm::vec3 face1[] =  {Cs[0], Cs[1], Cs[3], Cs[2]};
    glm::vec3 face2[] =  {Cs[4], Cs[5], Cs[7], Cs[6]};
    glm::vec3 face3[] =  {Cs[0], Cs[2], Cs[6], Cs[4]};
    glm::vec3 face4[] =  {Cs[1], Cs[3], Cs[7], Cs[5]};
    glm::vec3 face5[] =  {Cs[0], Cs[1], Cs[5], Cs[4]};
    glm::vec3 face6[] =  {Cs[2], Cs[3], Cs[7], Cs[6]};
    
//    float (*noise)(float x, float y, float z);
    noise = (*PlanetNoise);
    PColour = (*PlanetColour);
    
    squares.push_back(Square(face1, rad, noise, PColour));
    squares.push_back(Square(face2, rad, noise, PColour));
    squares.push_back(Square(face3, rad, noise, PColour));
    squares.push_back(Square(face4, rad, noise, PColour));
    squares.push_back(Square(face5, rad, noise, PColour));
    squares.push_back(Square(face6, rad, noise, PColour));
    
    std::thread threads[6];
    std::vector <Square> ToSet[6];
    for (int i = 0; i < squares.size(); i++)
    {
        threads[i] = std::thread(genSquares, squares[i], 7, std::ref(ToSet[i]), radius, noise, PColour);
    }
    for (int i = 0; i < 6; i++)
    {
        threads[i].join();
        for (int j = 0; j < ToSet[i].size(); j++)
        {
            setSquares.push_back(ToSet[i][j]);
        }
    }
    
    
    water = Circle(glm::vec3(0, 0, 0), rad, 7);
    
    std::vector <Square> atmosSquares;
    
    std::vector <Vertex> Verts;
    std::vector <GLuint> Inds;
    int offset = 0;
    for (int i = 0; i < squares.size(); i++)
    {
        std::vector <Square> temp = SubDividedN(squares[i], 0, 6, glm::vec3(0, 0, 0), atmoRad);
        for (int k = 0; k < temp.size(); k++)
        {
            Verts.insert( Verts.end(), temp[k].Verts.begin(), temp[k].Verts.end());
            for (int j = 0; j < temp[k].Inds.size(); j++)
            {
                Inds.push_back(temp[k].Inds[j] + offset);
            }
            offset += temp[k].Verts.size();
        }
    }
    atmos = Mesh(Verts, Inds);
}

std::vector <Square> Planet::SubDivided(Square& square, int depth, glm::vec3 pos)
{
    glm::vec3 distVec = pos - square.center;
    glm::vec3 relVec = glm::normalize(pos - position);
    
    float lin = (glm::dot(glm::normalize(pos - glm::normalize(square.center) * glm::mat3(radius)), relVec) + 1) / 2;
    float dist = glm::length(distVec) / pow(lin, 2);
    std::vector <Square> ToRet;
    if (lin > 0.4) {
        int goalDepth = fmin(1 / (0.05 * dist + 0.1), 4);
        if (goalDepth <= depth)
        {
            ToRet.push_back(square);
        } else {
            std::vector <Square> New = SubDivide(square, radius, noise, PColour);
            for (int i = 0; i < New.size(); i++)
            {
                std::vector <Square> ToAdd = SubDivided(New[i], depth + 1, pos);
                ToRet.insert(ToRet.end(), ToAdd.begin(), ToAdd.end());
            }
        }
    } else {
        ToRet.push_back(square);
    }
    return ToRet;
}

std::vector <Square> Planet::SubDivided(Square square, int depth, int goalDepth)
{
    std::vector <Square> ToRet;
    if (goalDepth <= depth)
    {
        ToRet.push_back(square);
    } else {
        std::vector <Square> New = SubDivide(square, radius, noise, PColour);
        for (int i = 0; i < New.size(); i++)
        {
            std::vector <Square> ToAdd = SubDivided(New[i], depth + 1, goalDepth);
            ToRet.insert(ToRet.end(), ToAdd.begin(), ToAdd.end());
        }
    }
    return ToRet;
}

std::vector <Square> Planet::SubDividedN(Square square, int depth, int goalDepth, glm::vec3 colour, float rad)
{
    std::vector <Square> ToRet;
    if (goalDepth <= depth)
    {
        ToRet.push_back(square);
    } else {
        std::vector <Square> New = SubDivide(square, rad, colour);
        for (int i = 0; i < New.size(); i++)
        {
            std::vector <Square> ToAdd = SubDividedN(New[i], depth + 1, goalDepth, colour, rad);
            ToRet.insert(ToRet.end(), ToAdd.begin(), ToAdd.end());
        }
    }
    return ToRet;
}

void SqsToVI(std::vector <Square>& Sqs, std::vector <Vertex>& Verts, std::vector <GLuint>& Inds, int& offset)
{
    for (int i = 0; i < Sqs.size(); i++)
    {
        Verts.insert( Verts.end(), Sqs[i].Verts.begin(), Sqs[i].Verts.end() );
        for (int j = 0; j < Sqs[i].Inds.size(); j++)
        {
            Inds.push_back(Sqs[i].Inds[j] + offset);
        }
        offset += Sqs[i].Verts.size();
    }
}

void Planet::CalculateMesh(glm::vec3 pos)
{
    std::vector <Vertex> Verts;
    std::vector <GLuint> Inds;
    int offset = 0;
    glm::vec3 relVec = glm::normalize(pos - position);
    glm::vec3 pCircPos = relVec * radius;
    float height = glm::length(pos - position - pCircPos * glm::mat3(1 + noise(pCircPos[0], pCircPos[1], pCircPos[2])));
    int gDep = 0;
    glm::vec3 hoverPos;
    
    if (height > 30) {
        hoverPos = relVec * glm::mat3(radius + 20);
        gDep = 1;
    } else if (height > 20) {
        hoverPos = relVec * glm::mat3(radius + 10);
        gDep = 1;
    } else if (height > 10) {
        hoverPos = relVec * glm::mat3(radius + 5);
        gDep = 2;
    } else {
        hoverPos = relVec * glm::mat3(radius + 2);
        gDep = 3;
    }
    
    std::vector <Square> ToDisp;
    
    for (int i = 0; i < setSquares.size(); i ++)
    {
        glm::vec3 CircPos = glm::normalize(setSquares[i].center);
        float lin = glm::dot(glm::normalize(hoverPos - CircPos * glm::mat3(radius)), CircPos);
        if (lin > 0)
        {
            setSquares[i].SubDivideSq(gDep, setSquares[i], radius, noise, PColour);
            ToDisp.push_back(setSquares[i]);
        } else if (lin > -0.2 and gDep > 1)
        {
            setSquares[i].SubDivideSq(gDep - 1, setSquares[i], radius, noise, PColour);
            ToDisp.push_back(setSquares[i]);
        } else if (lin > -0.4 and gDep > 2)
        {
            setSquares[i].SubDivideSq(gDep - 2, setSquares[i], radius, noise, PColour);
            ToDisp.push_back(setSquares[i]);
        } else if (lin > -0.6 and gDep > 3)
        {
            setSquares[i].SubDivideSq(gDep - 3, setSquares[i], radius, noise, PColour);
            ToDisp.push_back(setSquares[i]);
        } else {
            setSquares[i].SubDivideSq(0, setSquares[i], radius, noise, PColour);
            ToDisp.push_back(setSquares[i]);
        }
    }
    
    SqsToVI(ToDisp, Verts, Inds, offset);
    
    verts = Verts;
    inds = Inds;
//    mesh.Reset(verts, inds);
//    Mesh Random(Verts, Inds);
}

void Planet::SetMesh()
{
    mesh.Reset(verts, inds);
//    mesh.Delete();
//    mesh = Mesh(verts, inds);
}

void Planet::Draw(Camera &camera, Shader &atmosProgram, Shader &planetProgram, Shader &waterProgram, GLuint renderedTexture, Texture grass)
{
    float scat = 2;
    
    const glm::vec3 lightScat = glm::vec3(0.4 * scat, 0.6 * scat, 1 * scat);
    const float atmosThick = 0.05;
    const float densFall = 10;
    const glm::vec3 lightDirec = glm::vec3(-1, 0, 0);
    
    glDisable(GL_DEPTH_TEST);
    atmos.PDraw(atmosProgram, camera, position, radius, atmoRad, atmosThick, lightScat, densFall, lightDirec, false);
    glEnable(GL_DEPTH_TEST);
    
    grass.Bind();
    
    mesh.PDraw(planetProgram, camera, position, radius, atmoRad, atmosThick, lightScat, densFall, lightDirec, false);
    
    grass.Unbind();
    
    glBindTexture(glGetUniformLocation(waterProgram.ID, "renderedTexture"), renderedTexture);
    glUniform1i(glGetUniformLocation(waterProgram.ID, "renderedTexture"), 0);
    
    glDepthFunc(GL_LESS);
    water.PDraw(waterProgram, camera, position, radius, atmoRad, atmosThick, lightScat, densFall, lightDirec, false);
    
    glBindTexture(glGetUniformLocation(waterProgram.ID, "renderedTexture"), 0);
    
}

void Planet::DrawR(Camera &camera, Shader &atmosProgram, Shader &planetProgram, Texture grass)
{
    float scat = 2;
    
    const glm::vec3 lightScat = glm::vec3(0.4 * scat, 0.6 * scat, 1 * scat);
    const float atmosThick = 0.05;
    const float densFall = 10;
    const glm::vec3 lightDirec = glm::vec3(-1, 0, 0);
    
    glDisable(GL_DEPTH_TEST);
    atmos.PDraw(atmosProgram, camera, position, radius, atmoRad, atmosThick, lightScat, densFall, lightDirec, true);
    glEnable(GL_DEPTH_TEST);
    
    grass.Bind();
    mesh.PDraw(planetProgram, camera, position, radius, atmoRad, atmosThick, lightScat, densFall, lightDirec, true);
    grass.Unbind();
}
