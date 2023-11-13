#version 410 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec2 TexCoord;
uniform sampler2D Tex;

void main()
{
    FragColor = texture(Tex, TexCoord);//vec4(color, 1.0f);
}

