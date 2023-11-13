#version 410 core

// Outputs colors in RGBA
out vec4 FragColor;
//layout (location = 0) out vec4 color;


// Inputs the color from the Vertex Shader
in vec4 color;
in vec3 curPos;
in vec2 Tex;
in float weight;

uniform sampler2D renderedTexture;

uniform vec3 cameraPos;
uniform vec3 center;
uniform vec3 lightDirec;


void main()
{
    vec3 normal = normalize(curPos);
    float specularLight = 0.50f;
    vec3 viewDirection = normalize(cameraPos - curPos - center);
    vec3 reflectionDirection = reflect(lightDirec, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specAmount * specularLight;
    
//    FragColor = vec4(1, 0, 0, 1);
    FragColor = texture(renderedTexture, Tex);
    vec4 generalColour = vec4(min(color[0] + specular, 1), min(color[1] + specular, 1), min(color[2] + specular, 1), color[3]);
    
    FragColor *= mat4(1 - weight);
    FragColor += generalColour * mat4(weight);
    
//    color = vec4(1, 1, 1, 1);
}
