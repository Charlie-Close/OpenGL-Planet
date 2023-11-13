//
//  Circle.cpp
//  OpenGlTest
//
//  Created by Charlie Close on 25/03/2023.
//

#include"Shapes.h"
#include <iostream>
#include<cmath>

std::ostream &operator<<(std::ostream &os, Vertex const &m) {
    return os << "Position(" << m.position[0] << ", " << m.position[1] << ", " << m.position[2] << ") - Normal(" << m.normal[0] << ", " << m.normal[1] << ", " << m.normal[2] << ") - Colour(" << m.colour[0] << ", " << m.colour[1] << ", " << m.colour[2] << ")";
}

Mesh Circle(glm::vec3 position, float radius, int subDev)
{
    float s1 = radius / sqrt(1 + pow(((1 + sqrt(5)) / 2), 2));
    float sphi = s1 * (1 + sqrt(5)) / 2;
    
    float Colour[] = {0.8, 0.8, 0.8};
    Triangle StartingTriangles[20] = {
        Triangle(glm::vec3(0.f, sphi, s1) + position, glm::vec3(-s1, 0.f, sphi) + position, glm::vec3(s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, sphi, s1) + position, glm::vec3(sphi, s1, 0.f) + position, glm::vec3(s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, sphi, s1) + position, glm::vec3(sphi, s1, 0.f) + position, glm::vec3(0.f, sphi, -s1) + position, position, Colour),
        Triangle(glm::vec3(0.f, sphi, s1) + position, glm::vec3(-sphi, s1, 0.f) + position, glm::vec3(0.f, sphi, -s1) + position, position, Colour),
        Triangle(glm::vec3(0.f, sphi, s1) + position, glm::vec3(-sphi, s1, 0.f) + position, glm::vec3(-s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, s1) + position, glm::vec3(-s1, 0.f, sphi) + position, glm::vec3(s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, s1) + position, glm::vec3(sphi, -s1, 0.f) + position, glm::vec3(s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(sphi, s1, 0.f) + position, glm::vec3(sphi, -s1, 0.f) + position, glm::vec3(s1, 0.f, sphi) + position, position, Colour),
        Triangle(glm::vec3(sphi, s1, 0.f) + position, glm::vec3(sphi, -s1, 0.f) + position, glm::vec3(s1, 0.f, -sphi) + position, position, Colour),
        Triangle(glm::vec3(sphi, s1, 0.f) + position, glm::vec3(0.f, sphi, -s1) + position, glm::vec3(s1, 0.f, -sphi) + position, position, Colour),
        Triangle(glm::vec3(-s1, 0.f, -sphi) + position, glm::vec3(0.f, sphi, -s1) + position, glm::vec3(s1, 0.f, -sphi) + position, position, Colour),
        Triangle(glm::vec3(-s1, 0.f, -sphi) + position, glm::vec3(0.f, sphi, -s1) + position, glm::vec3(-sphi, s1, 0.f) + position, position, Colour),
        Triangle(glm::vec3(-s1, 0.f, -sphi) + position, glm::vec3(-sphi, -s1, 0.f) + position, glm::vec3(-sphi, s1, 0.f) + position, position, Colour),
        Triangle(glm::vec3(-s1, 0.f, sphi) + position, glm::vec3(-sphi, -s1, 0.f) + position, glm::vec3(0.f, -sphi, s1) + position, position, Colour),
        Triangle(glm::vec3(-s1, 0.f, sphi) + position, glm::vec3(-sphi, -s1, 0.f) + position, glm::vec3(-sphi, s1, 0.f) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, -s1) + position, glm::vec3(-sphi, -s1, 0.f) + position, glm::vec3(0.f, -sphi, s1) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, -s1) + position, glm::vec3(sphi, -s1, 0.f) + position, glm::vec3(0.f, -sphi, s1) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, -s1) + position, glm::vec3(sphi, -s1, 0.f) + position, glm::vec3(s1, 0.f, -sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, -s1) + position, glm::vec3(-s1, 0.f, -sphi) + position, glm::vec3(s1, 0.f, -sphi) + position, position, Colour),
        Triangle(glm::vec3(0.f, -sphi, -s1) + position, glm::vec3(-s1, 0.f, -sphi) + position, glm::vec3(-sphi, -s1, 0.f) + position, position, Colour),
    };
    
    Triangle* FinalTris = new Triangle[int(20 * pow(4, subDev))];
    
    for (int i = 0; i < 20; i++) {
        FinalTris[i] = StartingTriangles[i];
    }
    
    for (int j = 0; j < subDev; j++) {        
        Triangle* TTris2 = new Triangle[int(20 * pow(4, (j + 1)))];
        
        for (int i = 0; i < 20 * pow(4, j); i++) {
            FinalTris[i].Subdivide(&TTris2[i * 4], &TTris2[i * 4 + 1], &TTris2[i * 4 + 2], &TTris2[i * 4 + 3], position, radius);
        }
        for (int i = 0; i < 20 * pow(4, (j + 1)); i++) {
            FinalTris[i] = TTris2[i];
        }
    }
    
    std::vector <GLuint> IndWithReps;
    std::vector <Vertex> VertWithReps;
    
    for (int i = 0; i < 20 * pow(4, subDev); i++){
        IndWithReps.push_back(i * 3);
        IndWithReps.push_back(i * 3 + 1);
        IndWithReps.push_back(i * 3 + 2);
        for (int j = 0; j < 27; j+= 9) {
            VertWithReps.push_back(Vertex({
                glm::vec3(FinalTris[i].Vertices[j], FinalTris[i].Vertices[j + 1], FinalTris[i].Vertices[j + 2]),
                glm::vec3(FinalTris[i].Vertices[j + 3], FinalTris[i].Vertices[j + 4], FinalTris[i].Vertices[j + 5]),
                glm::vec3(FinalTris[i].Vertices[j + 6], FinalTris[i].Vertices[j + 7], FinalTris[i].Vertices[j + 8])}));
        }
    }
    
    Mesh Mesh1(VertWithReps, IndWithReps);
    return Mesh1;
};

Vertex CreateVert(glm::vec3 vec, float radius, float (*noise)(float x, float y, float z), glm::vec3 (*nToC)(float height, glm::vec3 norm, glm::vec3 pos))
{
    float eps = 0.03;
    
    glm::vec3 r = glm::normalize(vec) * glm::mat3(radius);
    float n = noise(r[0], r[1], r[2]);
    glm::vec3 v = r * glm::mat3(n + 1);
    
    glm::vec3 horiz(r[2], 0, -r[0]);
    glm::vec3 vert = glm::cross(horiz, r);
    
    glm::vec3 rx(r[0] + eps, r[1], r[2]);
    glm::vec3 ry(r[0], r[1] + eps, r[2]);
    glm::vec3 rz(r[0], r[1], r[2] + eps);
    
    glm::vec3 v_x = rx * glm::mat3(noise(rx[0], rx[1], rx[2]) + 1) - v;
    glm::vec3 v_y = ry * glm::mat3(noise(ry[0], ry[1], ry[2]) + 1) - v;
    glm::vec3 v_z = rz * glm::mat3(noise(rz[0], rz[1], rz[2]) + 1) - v;
    
    glm::vec3 vHoriz = v_x * glm::mat3(horiz[0]) + v_y * glm::mat3(horiz[1]) + v_z * glm::mat3(horiz[2]);
    glm::vec3 vVert = v_x * glm::mat3(vert[0]) + v_y * glm::mat3(vert[1]) + v_z * glm::mat3(vert[2]);
    
    glm::vec3 norm = glm::normalize(glm::cross(vHoriz, vVert));
    if (glm::dot(norm, v) < 0)
    {
        norm = norm * glm::mat3(-1);
    }
    
    return Vertex({v, norm, nToC(n, norm, r)});
}

Square::Square(glm::vec3 verts[4], float radius, float (*noise)(float x, float y, float z), glm::vec3 (*nToC)(float height, glm::vec3 norm, glm::vec3 pos))
{
    SubDivs = 0;
    for (int i = 0; i < 4; i++)
    {
        Corners.push_back(verts[i]);
    }
    Verts.push_back(CreateVert(verts[0], radius, noise, nToC));
    Verts.push_back(CreateVert(verts[1], radius, noise, nToC));
    Verts.push_back(CreateVert(verts[2], radius, noise, nToC));
    Verts.push_back(CreateVert(verts[3], radius, noise, nToC));
    Verts.push_back(CreateVert(verts[0] + verts[2], radius, noise, nToC));
    
    OgVerts = Verts;
    
    Inds.push_back(0);
    Inds.push_back(1);
    Inds.push_back(4);
    
    Inds.push_back(1);
    Inds.push_back(2);
    Inds.push_back(4);
    
    Inds.push_back(2);
    Inds.push_back(3);
    Inds.push_back(4);
    
    Inds.push_back(3);
    Inds.push_back(0);
    Inds.push_back(4);
    
    avgNorm = (Verts[0].normal + Verts[1].normal + Verts[2].normal + Verts[3].normal + Verts[4].normal) * glm::mat3(0.2);
    center = Verts[4].position;
}

Square::Square(glm::vec3 verts[4], float radius, glm::vec3 colour)
{
    SubDivs = 0;
    for (int i = 0; i < 4; i++)
    {
        Corners.push_back(verts[i]);
    }
    
    glm::vec3 v0 = glm::normalize(verts[0]) * glm::mat3(radius);
    glm::vec3 v1 = glm::normalize(verts[1]) * glm::mat3(radius);
    glm::vec3 v2 = glm::normalize(verts[2]) * glm::mat3(radius);
    glm::vec3 v3 = glm::normalize(verts[3]) * glm::mat3(radius);
    glm::vec3 v4 = glm::normalize((verts[0] + verts[2]) * glm::mat3(0.5)) * glm::mat3(radius);
    
    glm::vec3 norm1 = glm::normalize(glm::cross(v4 - v0, v4 - v1));
    if (glm::dot(norm1, v4) < 0)
    {
        norm1 = norm1 * glm::mat3(-1);
    }
    Verts.push_back(Vertex({v0, norm1, colour}));
    Verts.push_back(Vertex({v4, norm1, colour}));
    Verts.push_back(Vertex({v1, norm1, colour}));
    
    glm::vec3 norm2 = glm::normalize(glm::cross(v4 - v1, v4 - v2));
    if (glm::dot(norm2, v4) < 0)
    {
        norm2 = norm2 * glm::mat3(-1);
    }
    Verts.push_back(Vertex({v1, norm2, colour}));
    Verts.push_back(Vertex({v4, norm2, colour}));
    Verts.push_back(Vertex({v2, norm2, colour}));
    
    glm::vec3 norm3 = glm::normalize(glm::cross(v4 - v2, v4 - v3));
    if (glm::dot(norm3, v4) < 0)
    {
        norm3 = norm3 * glm::mat3(-1);
    }
    Verts.push_back(Vertex({v2, norm3, colour}));
    Verts.push_back(Vertex({v4, norm3, colour}));
    Verts.push_back(Vertex({v3, norm3, colour}));
    
    glm::vec3 norm4 = glm::normalize(glm::cross(v4 - v3, v4 - v0));
    if (glm::dot(norm4, v4) < 0)
    {
        norm4 = norm4 * glm::mat3(-1);
    }
    Verts.push_back(Vertex({v3, norm4, colour}));
    Verts.push_back(Vertex({v4, norm4, colour}));
    Verts.push_back(Vertex({v0, norm4, colour}));
    
    for (int i = 0; i < 12; i++)
    {
        Inds.push_back(i);
    }
    
    avgNorm = (norm1 + norm2 + norm3 + norm4) * glm::mat3(0.25);
    
    center = v4;
}

Square::Square(Vertex verts[5], glm::vec3 corners[4])
{
    SubDivs = 0;
    for (int i = 0; i < 5; i++)
    {
        Verts.push_back(verts[i]);
    }
    
    for (int i = 0; i < 4; i++)
    {
        Corners.push_back(corners[i]);
    }
    OgVerts = Verts;
    
    Inds.push_back(0);
    Inds.push_back(1);
    Inds.push_back(4);
    
    Inds.push_back(1);
    Inds.push_back(2);
    Inds.push_back(4);
    
    Inds.push_back(2);
    Inds.push_back(3);
    Inds.push_back(4);
    
    Inds.push_back(3);
    Inds.push_back(0);
    Inds.push_back(4);
    
    avgNorm = (Verts[0].normal + Verts[1].normal + Verts[2].normal + Verts[3].normal + Verts[4].normal) * glm::mat3(0.2);
    center = Verts[4].position;
}

Square::Square(std::vector <Vertex> verts, std::vector <glm::vec3> corners)
{
    SubDivs = 0;
    Verts = verts;
    Corners = corners;
    OgVerts = verts;
    
    Inds.push_back(0);
    Inds.push_back(1);
    Inds.push_back(4);
    
    Inds.push_back(1);
    Inds.push_back(2);
    Inds.push_back(4);
    
    Inds.push_back(2);
    Inds.push_back(3);
    Inds.push_back(4);
    
    Inds.push_back(3);
    Inds.push_back(0);
    Inds.push_back(4);
    
    avgNorm = (Verts[0].normal + Verts[1].normal + Verts[2].normal + Verts[3].normal + Verts[4].normal) * glm::mat3(0.2);
    center = Verts[4].position;
}

void Square::SubDivideSq(int subs, Square& square, float radius, float (*noise)(float x, float y, float z), glm::vec3 (*nToC)(float height, glm::vec3 norm, glm::vec3 pos))
{
    if (subs > SubDivs)
    {
        std::vector <Square> NewSquares;
        if (SubDivs == 0) {
            Square Resetting(OgVerts, Corners);
            
            NewSquares = SubDivide(Resetting, radius, noise, nToC);
            SubDivs = 1;
        } else {
            for (int i = 0; i < pow(4, SubDivs); i++) {
                std::vector <Vertex> ToAddVerts = {SubedVerts[5 * i], SubedVerts[5 *i + 1], SubedVerts[5 *i + 2], SubedVerts[5 *i + 3], SubedVerts[5 *i + 4]};
                std::vector <glm::vec3> ToAddCorners = {SubedCorners[4 * i], SubedCorners[4 * i + 1], SubedCorners[4 * i + 2], SubedCorners[4 * i + 3]};
                NewSquares.push_back(Square(ToAddVerts, ToAddCorners));
            }
        }
        
        
        while (subs > SubDivs)
        {
            std::vector <Square> setSquares;
            for (int i = 0; i < NewSquares.size(); i++)
            {
                std::vector <Square> ToAdd = SubDivide(NewSquares[i], radius, noise, nToC);
                setSquares.insert( setSquares.end(), ToAdd.begin(), ToAdd.end());
            }
            NewSquares = setSquares;
            SubDivs += 1;
        }
        
        Verts.clear();
        Inds.clear();
        SubedVerts.clear();
        SubedCorners.clear();
        int offset = 0;
        
        for (int i = 0; i < NewSquares.size(); i++)
        {
            Verts.insert( Verts.end(), NewSquares[i].Verts.begin(), NewSquares[i].Verts.end());
            SubedCorners.insert( SubedCorners.end(), NewSquares[i].Corners.begin(), NewSquares[i].Corners.end());
            for (int j = 0; j < NewSquares[i].Inds.size(); j++)
            {
                Inds.push_back(NewSquares[i].Inds[j] + offset);
            }
            offset += NewSquares[i].Verts.size();
        }
        SubedVerts = Verts;
    } else if (SubDivs != 0 and subs == 0) {
        Square Resetting(OgVerts, Corners);
        SubedVerts.clear();
        SubedCorners.clear();
        Verts = Resetting.Verts;
        Inds = Resetting.Inds;
        SubDivs = 0;
    }
}

std::vector <Square> SubDivide(Square& square, float radius, float (*noise)(float x, float y, float z), glm::vec3 (*nToC)(float height, glm::vec3 norm, glm::vec3 pos))
{
    std::vector <Square> squares;
    glm::vec3 c0_2 = (square.Corners[0] + square.Corners[1]) * glm::mat3(0.5);
    glm::vec3 c2_4 = (square.Corners[1] + square.Corners[2]) * glm::mat3(0.5);
    glm::vec3 c4_2 = (square.Corners[2] + square.Corners[3]) * glm::mat3(0.5);
    glm::vec3 c2_0 = (square.Corners[3] + square.Corners[0]) * glm::mat3(0.5);
    glm::vec3 c2_2 = (square.Corners[0] + square.Corners[2]) * glm::mat3(0.5);
    
    Vertex mid0_2 = CreateVert(c0_2, radius, noise, nToC);
    Vertex mid1_3 = CreateVert(c0_2 + c2_4, radius, noise, nToC);
    Vertex mid2_4 = CreateVert(c2_4, radius, noise, nToC);
    Vertex mid3_3 = CreateVert(c2_4 + c4_2, radius, noise, nToC);
    Vertex mid4_2 = CreateVert(c4_2, radius, noise, nToC);
    Vertex mid3_1 = CreateVert(c2_0 + c4_2, radius, noise, nToC);
    Vertex mid2_0 = CreateVert(c2_0, radius, noise, nToC);
    Vertex mid1_1 = CreateVert(c0_2 + c2_0, radius, noise, nToC);
    
    Vertex sq1[] = {square.Verts[0], mid0_2, square.Verts[4], mid2_0, mid1_1};
    glm::vec3 c1[] = {square.Corners[0], c0_2, c2_2, c2_0};
    Vertex sq2[] = {mid0_2, square.Verts[1], mid2_4, square.Verts[4], mid1_3};
    glm::vec3 c2[] = {c0_2, square.Corners[1], c2_4, c2_2};
    Vertex sq3[] = {square.Verts[4], mid2_4, square.Verts[2], mid4_2, mid3_3};
    glm::vec3 c3[] = {c2_2, c2_4, square.Corners[2], c4_2};
    Vertex sq4[] = {mid2_0, square.Verts[4], mid4_2, square.Verts[3], mid3_1};
    glm::vec3 c4[] = {c2_0, c2_2, c4_2, square.Corners[3]};
    
    squares.push_back(Square(sq1, c1));
    squares.push_back(Square(sq2, c2));
    squares.push_back(Square(sq3, c3));
    squares.push_back(Square(sq4, c4));
    
    return squares;
}

std::vector <Square> SubDivide(Square square, float radius, glm::vec3 colour)
{
    std::vector <Square> squares;
    glm::vec3 mid0 = (square.Corners[0] + square.Corners[2]) * glm::mat3(0.5);
    glm::vec3 mid1 = (square.Corners[0] + square.Corners[1]) * glm::mat3(0.5);
    glm::vec3 mid2 = (square.Corners[1] + square.Corners[2]) * glm::mat3(0.5);
    glm::vec3 mid3 = (square.Corners[2] + square.Corners[3]) * glm::mat3(0.5);
    glm::vec3 mid4 = (square.Corners[3] + square.Corners[0]) * glm::mat3(0.5);
    
    glm::vec3 sq1[] = {square.Corners[0], mid4, mid0, mid1};
    glm::vec3 sq2[] = {square.Corners[1], mid1, mid0, mid2};
    glm::vec3 sq3[] = {square.Corners[2], mid2, mid0, mid3};
    glm::vec3 sq4[] = {square.Corners[3], mid3, mid0, mid4};
    
    squares.push_back(Square(sq1, radius, colour));
    squares.push_back(Square(sq2, radius, colour));
    squares.push_back(Square(sq3, radius, colour));
    squares.push_back(Square(sq4, radius, colour));
    
    return squares;
}
