#version 150 core

in vec2 TextureCoords
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
    float dx = dFdx(position);
    float dy = dFdy(position);
    vec3 facetNormal = normalize(cross(dy, dx));

    vec3 reflection = -lightDirection + 2 * facetNormal;
    
    vec3 colorAmbient = 
        texture(textureDiffuse, TextureCoords)
        * ambientColor 
        * ambientIntensity;

    vec3 colorDiffuse = 
        texture(textureDiffuse, TextureCoords) 
        * max(dot(facetNormal, lightDirection), 0)
        * lightColor 
        * lightIntensity;

    vec3 colorSpecular =
        texture(textureSpecular, TextureCoords)
        * pow(max(dot(reflection, -normalize(position))), texture(textureSpecularExp, TextureCoords))
        * lightColor
        * lightIntensity;

    vec3 colorEmissive = 
        texture(textureEmissive, TextureCoords);

    outColor = colorAmbient + colorDiffuse + colorSpecular + colorEmissive;
}