#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragPosition;
in vec3 fragNormal;

// Input uniform values
uniform vec4 colDiffuse;
uniform sampler2D texture0;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Sample the texture with the tiled coordinates
    vec4 texelColor = texture(texture0, fragTexCoord);
    
    finalColor = texelColor;
}