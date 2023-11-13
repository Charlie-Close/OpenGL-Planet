#version 410 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 norm;

layout (location = 2) in vec3 aColour;

uniform vec3 center;
uniform float radius;
uniform vec3 cameraPos;
uniform float atmosRad;
uniform vec3 atmosCol;
uniform vec3 lightDirec;
uniform float atmosThick;
uniform float densFall;

uniform sampler2D renderedTexture;


// Outputs the color for the Fragment Shader
out vec4 color;
out vec3 curPos;
out float weight;
out vec2 Tex;

uniform mat4 camMatrix;


vec3 lightColour = vec3(1.0, 1.0, 1.0);

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

float fit(float val)
{
    return min(max(val, 0.00001), 0.99999);
}

void main()
{
    curPos = aPos;
    // Outputs the positions/coordinates of all vertices
    gl_Position = camMatrix * vec4(aPos + center, 1);
    // Assigns the colors from the Vertex Data to "color"
    color = vec4(vec3(0.1, 0.2, 0.6) * mat3(max(-dot(norm, lightDirec), 0.2)), 1 - max(dot(normalize(cameraPos - center), norm), 0) * 0.1);
    vec3 ndc = gl_Position.xyz / gl_Position.w;
    Tex = vec2(fit((ndc.x + 1) / 2), fit((ndc.y + 1) / 2));
    
    vec3 relativePos = cameraPos - center;

    vec3 lookNorm = normalize(aPos - relativePos);

    vec2 DistThroughAtmos = raySphere(cameraPos - center, lookNorm, atmosRad);

    weight = max(min(abs(dot(lookNorm, norm)) * 1.5 + length(relativePos - aPos) / 50, 1), 0);

    if (dot(lookNorm, norm) < 0)
    {
        if (length(relativePos) > atmosRad)
        {
            vec3 rayOrig = relativePos + lookNorm * mat3(DistThroughAtmos[1]);
            color = vec4(calculateLight(rayOrig, lookNorm, length(rayOrig - aPos), vec3(color)), color[3]);
        } else {
            color = vec4(calculateLight(relativePos, lookNorm, length(relativePos - aPos), vec3(color)), color[3]);
        }
    }
}


