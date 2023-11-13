#version 410 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 norm;

layout (location = 2) in vec3 aColour;


// Outputs the color for the Fragment Shader
out vec3 color;

uniform mat4 camMatrix;
uniform vec3 offset;

void main()
{
    // Outputs the positions/coordinates of all vertices
    gl_Position = camMatrix * vec4(aPos + offset, 1.0);
    // Assigns the colors from the Vertex Data to "color"
    color = vec3(aColour[0], aColour[1], aColour[2]) * mat3(max(dot(norm, vec3(-1, 0, 0)), 0.2));
    // Assigns the texture coordinates from the Vertex Data to "texCoord"
}

