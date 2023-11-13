#version 410 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 norm;

layout (location = 2) in vec3 aColour;


// Outputs the color for the Fragment Shader
out vec2 TexCoord;

uniform mat4 camMatrix;
uniform vec3 offset;

uniform mat4 lknee;
uniform mat4 rknee;
uniform mat4 lhip;
uniform mat4 rhip;
uniform mat4 lsho;
uniform mat4 rsho;
uniform mat4 lelb;
uniform mat4 relb;

void main()
{
    // Outputs the positions/coordinates of all vertices
    vec4 usePos = vec4(aPos, 1);
    if (aColour[2] == 0.01) {
        usePos[1] += 0.0008;
        usePos *= lknee;
        usePos[1] -= 0.0008;
    }
    if (aColour[2] == 0.03) {
        usePos[1] += 0.0008;
        usePos *= (mat4(1) + lknee);
        usePos *= 0.5;
        usePos[1] -= 0.0008;
    }
    if (aColour[2] == 0.01 || aColour[2] == 0.03 || aColour[2] == 0.05) {
        usePos[1] += 0.0002;
        usePos *= lhip;
        usePos[1] -= 0.0002;
    }
    if (aColour[2] == 0.07) {
        usePos[1] += 0.0002;
        usePos *= (mat4(1) + lhip);
        usePos *= 0.5;
        usePos[1] -= 0.0002;
    }
    
    gl_Position = camMatrix * vec4(vec3(usePos) + offset, 1.0);
    // Assigns the colors from the Vertex Data to "color"
    TexCoord = vec2(aColour[0], aColour[1]);//vec3(aColour[0], aColour[1], aColour[2]) * mat3(max(dot(norm, vec3(-1, 0, 0)), 0.2));
    // Assigns the texture coordinates from the Vertex Data to "texCoord"
}


