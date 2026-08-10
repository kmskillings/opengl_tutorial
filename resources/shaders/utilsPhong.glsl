#version 150 core

vec3 getComponentAmbient(
    vec3 colorAmbient,
    vec3 lightAmbientColor
)
{
    return colorAmbient * lightAmbientColor;
}

vec3 getComponentDiffuse(
    vec3 colorDiffuse,
    vec3 normal,
    vec3 lightDirectionalColor,
    vec3 lightDirection
)
{
    return colorDiffuse
        * lightDirectionalColor
        * max(dot(lightDirection, normal), 0);
}

vec3 getComponentSpecular(
    vec3 colorSpecular,
    float specularPower,
    vec3 normal,
    vec3 lightDirectionalColor,
    vec3 lightDirection,
    vec3 position
)
{
    vec3 reflection = -lightDirection + 2 * (dot(normal, lightDirection) * normal);
    float specularStrength = max(dot(reflection, -normalize(position)), 0);
    return colorSpecular
        * pow(specularStrength, specularPower)
        * lightDirectionalColor;
}