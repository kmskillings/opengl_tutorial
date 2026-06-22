#version 150 core

in vec2 TextureCoords;
in vec3 Position; // In view space

uniform sampler2D textureDiffuse;
uniform sampler2D textureSpecular;
uniform sampler2D textureSpecularExp;
uniform sampler2D textureEmissive;

uniform vec3 ambientColor;

uniform vec3 lightDirection;    // In view space. The direction TO the light source.
uniform vec3 lightColor;

out vec4 outColor;

void main()
{
    vec3 dx = dFdx(Position);
    vec3 dy = dFdy(Position);
    vec3 facetNormal = normalize(cross(dx, dy));

    vec3 reflection = -lightDirection + 2 * (dot(facetNormal, lightDirection) * facetNormal);
    
    vec3 colorDiffuse = texture(textureDiffuse, TextureCoords).xyz;

    vec3 componentAmbient = 
        colorDiffuse
        * ambientColor;

    vec3 componentDiffuse =
        colorDiffuse
        * max(dot(facetNormal, lightDirection), 0)
        * lightColor;

    vec3 colorSpecular = texture(textureSpecular, TextureCoords).xyz;
    float specularExp = texture(textureSpecularExp, TextureCoords).x;
    float specularStrength = max(dot(reflection, -normalize(Position)), 0);
    vec3 componentSpecular =
        colorSpecular
        * pow(specularStrength, specularExp)
        * lightColor;

    vec3 componenetEmissive = 
        texture(textureEmissive, TextureCoords).xyz;

    vec3 componentTotal = 
        componentAmbient 
        + componentDiffuse
        + componentSpecular 
        + componenetEmissive;

    outColor = vec4(componentTotal, 1.0);
}