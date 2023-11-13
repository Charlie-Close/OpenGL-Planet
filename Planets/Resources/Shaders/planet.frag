#version 410 core

// Outputs colors in RGBA
out vec4 FragColor;
in vec2 texCoord;

//in vec3 rayOrigin;
//in vec3 rayDir;
//in float rayLength;
in vec3 normal;
in mat3 colorMult;
in vec3 color;


uniform vec3 center;
uniform float radius;
uniform float atmosRad;
uniform vec3 atmosCol;
uniform vec3 lightDirec;
uniform float atmosThick;
uniform float densFall;
vec3 lightColour = vec3(1.0, 1.0, 1.0);
uniform sampler2D tex0;

// Inputs the color from the Vertex Shader
//in vec3 color;

vec2 raySphere(vec3 origin, vec3 direction, float radius)
{
    vec3 offset = origin - center;
    const float a = 1;
    float b = 2 * dot (offset, direction);
    float c = dot (offset, offset) - radius * radius;

    float discriminant = b*b - 4*a*c;

    if (discriminant > 0)
    {
        float s = sqrt(discriminant);
        float dstToSphereNear = max(0, (-b - s) / (2 * a));
        float dstToSphereFar = (-b + s) / (2 * a);

        if (dstToSphereFar >= 0) {
            return vec2(max(dstToSphereFar - dstToSphereNear, 0), dstToSphereNear);
        }
    }
    return vec2(0, 0);
}


float numInScatteringPoints = 7;


float densityAtPoint(vec3 densitySamplePoint) {
    float heightAboveSurface = length(densitySamplePoint) - radius;
    float height01 = heightAboveSurface / (atmosRad - radius);
    float localDensity = exp(-height01 * densFall) * (1 - height01);
    return localDensity * atmosThick;
}

float opticalDepth(vec3 rayOrigin, vec3 rayDir, float rayLength)
{
    vec3 densitySamplePoint = rayOrigin;
    float stepSize = rayLength / (numInScatteringPoints - 1);
    float opticalDepth = 0;
    
    for (int i = 0; i < numInScatteringPoints; i ++)
    {
        float localDensity = densityAtPoint(densitySamplePoint);
        opticalDepth += localDensity;
        densitySamplePoint += rayDir * mat3(stepSize);
    }
    
    return opticalDepth * stepSize;
}


vec3 calculateLight(vec3 rayOrigin, vec3 rayDir, float rayLength, vec3 originalCol)
{
    vec3 inScatterPoint = rayOrigin;
    float stepSize = rayLength / (numInScatteringPoints - 1);
    vec3 inScatteredLight = vec3(0, 0, 0);
    float viewRayOpticalDepth = 0;
    vec3 dirToSun = lightDirec * mat3(-1);
    
    for (int i = 0; i < numInScatteringPoints; i ++)
    {
        float sunRayLength = raySphere(inScatterPoint, dirToSun, atmosRad)[0];
        float sunRayOpticalDepth = opticalDepth(inScatterPoint, dirToSun, sunRayLength);
        viewRayOpticalDepth = opticalDepth(inScatterPoint, mat3(-1)*rayDir, stepSize * i);
        vec3 transmittance = vec3(exp(-(sunRayOpticalDepth + viewRayOpticalDepth) * atmosCol[0]), exp(-(sunRayOpticalDepth + viewRayOpticalDepth) * atmosCol[1]), exp(-(sunRayOpticalDepth + viewRayOpticalDepth) * atmosCol[2]));
        float localDensity = densityAtPoint(inScatterPoint);
        mat3 absobConst = mat3(localDensity * stepSize);
        
        inScatteredLight += absobConst * transmittance * atmosCol;
        inScatterPoint += mat3(stepSize) * rayDir;
    }
    float originalColTransmittance = exp(-viewRayOpticalDepth);
    return originalCol * mat3(originalColTransmittance) + inScatteredLight * lightColour;
}

void main()
{
    vec4 texCol = texture(tex0, texCoord) * mat4(max(-dot(normal, lightDirec), 0.2));
    
    FragColor = vec4(vec3(texCol) * colorMult + color, 1);//texture(tex, texCoord);//vec4(color, 1.0f);
}
