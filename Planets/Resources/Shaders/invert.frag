#version 410 core
layout(location = 0) out vec3 FragColor;
  
in vec4 color;

void main()
{
    FragColor = vec3(color);
}
