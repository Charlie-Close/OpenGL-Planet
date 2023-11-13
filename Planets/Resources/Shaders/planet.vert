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
uniform bool invert;

vec3 lightColour = vec3(1.0, 1.0, 1.0);


// Outputs the color for the Fragment Shader
out mat3 colorMult;
out vec3 color;
out vec2 texCoord;
out vec3 normal;
//out vec3 rayOrigin;
//out vec3 rayDir;
//out float rayLength;

uniform mat4 camMatrix;

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


vec3 calculateLight(vec3 rayOrigin, vec3 rayDir, float rayLength)
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
    return inScatteredLight * lightColour;
}

float pathLength(vec3 a, vec3 b, float lerp)
{
    vec3 mid = a * mat3(lerp) + b * mat3(1 - lerp);
    vec3 reflectNorm = normalize(mid);
    vec3 reflectPoint = reflectNorm * mat3(radius);
    
    return abs(dot(mid, normalize(a - reflectPoint)) - dot(mid, normalize(b - reflectPoint)));
    
//    return length(a - reflectPoint) + length(b - reflectPoint);
}

vec3 MinimizePath(vec3 a, vec3 b)
{
//    float la = length(a);
//    float lb = length(b);
//
    float lerp = 0.5;
    float score = pathLength(a, b, lerp);
    float delta = 0.1;
    int direc = 0;
    
    while (delta >= 0.0005) {
        if (direc == 0) {
            lerp += delta;
            float newScore = pathLength(a, b, lerp);
            if (newScore < score) {
                score = newScore;
            } else {
                lerp -= 2 * delta;
                float newScore = pathLength(a, b, lerp);
                if (newScore < score) {
                    score = newScore;
                } else {
                    lerp += delta;
                    delta *= 0.25;
                }
            }
        }
    }
    
    return normalize(a * mat3(lerp) + b * mat3(1 - lerp)) * mat3(radius * 1.001);
}


void main()
{
    normal = norm;
    // Outputs the positions/coordinates of all vertices
    gl_Position = camMatrix * vec4(aPos + center, 1);
    // Assigns the colors from the Vertex Data to "color"
    vec3 relativePos = cameraPos - center;
    
    vec3 lookNorm = normalize(aPos - relativePos);
    
    color = vec3(aColour[0], aColour[1], aColour[2]) * mat3(max(-dot(norm, lightDirec), 0.2));
    
    if (dot(lookNorm, norm) < 0)
    {
        vec2 DistThroughAtmos = raySphere(cameraPos - center, lookNorm, atmosRad);
                    
        if (length(relativePos) > atmosRad)
        {
            vec3 rayOrig = relativePos + lookNorm * mat3(DistThroughAtmos[1]);
            color = calculateLight(rayOrig, lookNorm, length(rayOrig - aPos));
            float viewRayOpticalDepth = opticalDepth(aPos, mat3(-1)*lookNorm, length(rayOrig - aPos));
            colorMult = mat3(exp(-viewRayOpticalDepth));
            
        } else {
            color = calculateLight(relativePos, lookNorm, length(relativePos - aPos));
            float viewRayOpticalDepth = opticalDepth(aPos, mat3(-1)*lookNorm, length(relativePos - aPos));
            colorMult = mat3(exp(-viewRayOpticalDepth));
        }
        
    }
    
    texCoord = vec2(aPos[0] * 10, aPos[1] * 10);
    
    if (invert) {
        vec3 usePos = aPos;
        if (length(aPos) < radius) {
            usePos = normalize(aPos) * mat3(radius);
        }

        vec3 mid = MinimizePath(usePos, relativePos);

        if (dot(aPos, aPos - relativePos) < 0 && length(aPos) >= radius) {
            vec3 lookVect = normalize(mid - relativePos);
            float dist = length(relativePos - mid) + length(usePos - mid);
            gl_Position = camMatrix * vec4(cameraPos + lookVect * mat3(dist), 1);
        } else {
            vec3 lookVect = normalize(mid - relativePos);
            float dist = length(relativePos - mid) + length(usePos - mid);
            gl_Position = camMatrix * vec4(cameraPos + lookVect * mat3(dist), 1);

            gl_Position[2] = 10000;
        }
    }
        
    // Assigns the texture coordinates from the Vertex Data to "texCoord"
}
