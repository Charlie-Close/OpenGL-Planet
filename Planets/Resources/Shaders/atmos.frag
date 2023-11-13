#version 410 core

// Outputs colors in RGBA
out vec4 FragColor;
//layout (location = 0) out vec4 color;


// Inputs the color from the Vertex Shader
in vec4 color;
in vec3 curPos;

uniform vec3 cameraPos;
uniform vec3 center;


void main()
{
    FragColor = color;
//    color = vec4(1, 1, 1, 1);
}

