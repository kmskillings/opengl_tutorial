#version 460 core

in vec3 Position;       // Given in view space
in vec2 TextureCoords;

out vec4 outColor;

layout(location = 0) uniform mat4 matrixProj;
layout(location = 1) uniform mat4 matrixView;
layout(location = 3) uniform sampler2D textureMain;
layout(location = 4) uniform vec3 spherePosition;
layout(location = 5) uniform float sphereRadius;

void main()
{
    vec3 spherePositionView = (matrixView * vec4(spherePosition, 1.0f)).xyz;
    vec3 distanceFromSphere = Position - spherePositionView;
    if (
        dot(distanceFromSphere, distanceFromSphere) < 
        sphereRadius * sphereRadius
    )
    {
        discard;
    } else {
        if (gl_FrontFacing)
        {
            outColor = texture(textureMain, TextureCoords);
            gl_FragDepth = gl_FragCoord.z;
        }
        else
        {
            // Determine whether an intersection with the sphere exists
            float a = dot(Position, Position);
            float b = -2 * dot(spherePositionView, Position);
            float c = dot(spherePositionView, spherePositionView) - sphereRadius * sphereRadius;
            float discriminant = b * b - 4 * a * c;
            if (discriminant <= 0.0f)
            {
                // Only zero or one intersections exist, so we shouldn't see
                // it.
                discard;
            }
            else
            {
                // Two intersections exist, so get the rearmost one.
                float t = (-b + sqrt(discriminant)) / (2 * a);
                vec3 intersectionView = t * Position;

                // If the intersection is further away than the position of the
                // actual fragment being rendered, discard it.
                if (dot(intersectionView, intersectionView) > dot(Position, Position))
                {
                    discard;
                }
                else
                {
                    outColor = vec4(0.0, 1.0, 0.0, 1.0);
                    // Determine the screen-space depth of the intersection point
                    vec4 intersectionClip = matrixProj * vec4(intersectionView, 1.0);
                    float depthNdc = intersectionClip.z / intersectionClip.w;
                    // gl_FragDepth = ((gl_DepthRange.diff * depthNdc) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;
                    gl_FragDepth = gl_FragCoord.z;
                }       
            }
        }
    }
}