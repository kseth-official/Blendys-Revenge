#version 330

// From vertex shader
in vec2 texcoord;
in vec3 vcsPosition;

// Application data
uniform sampler2D sampler0;
uniform sampler2D normal_map;
uniform bool usesNormalMap;
uniform vec3 fcolor;

// Giant Shading
uniform int isGiant;

// Eye Position
uniform vec3 cameraPosition;       // The position of the eye

// Lighting information
uniform vec3 lightPosition;     // Position of the light source
uniform vec3 lightColor;        // Color of the light source
uniform float shininess;        // Shininess parameter for specular reflection
uniform float ambientIntensity; // Strength of ambient lighting

// Time
uniform float time;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
    vec4 texColor = texture(sampler0, texcoord);
    vec3 ambientColor = lightColor * ambientIntensity;
    
    vec3 N;
    if (usesNormalMap) {
        N = normalize(texture(normal_map, texcoord).xyz * 2.0 - 1.0); // Normalize N map
    } else {
        color = vec4(fcolor + ambientColor, 1.0) * texture(sampler0, vec2(texcoord.x, texcoord.y));
        return;
    }
        
    // Calculate light direction
    vec3 lightDir = normalize(lightPosition - vcsPosition);

    // Calculate view direction (assuming camera at cameraPosition)
    vec3 viewDir = normalize(cameraPosition - vcsPosition);

    // Calculate halfway vector
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // Calculate diffuse and specular components using Blinn-Phong model
    float diffuseIntensity = max(dot(N, lightDir), 0.0);
    float specularIntensity = pow(max(dot(N, halfwayDir), 0.0), shininess);

    // Combine ambient, diffuse, and specular components with light color
    vec3 diffuseColor = lightColor * diffuseIntensity;
    vec3 specularColor = lightColor * specularIntensity;
    vec3 finalColor;

    if (isGiant == 1) {
        vec3 giantColor = vec3(clamp(sin(time * fcolor.x),0.0,1.0), clamp(cos(time * fcolor.y),0.0,1.0), tan(time * fcolor.z));
        finalColor =  ambientColor + (giantColor * texColor.rgb * diffuseColor) + specularColor;
    } else {
        finalColor =  ambientColor + (fcolor * texColor.rgb * diffuseColor) + specularColor;    
    }
    
    color = vec4(finalColor, texColor.a);
}

