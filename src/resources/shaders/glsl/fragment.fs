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

// Custom uniform variables
uniform float texels_per_unit;  // How many texture pixels per world unit
uniform vec2 texture_size;     // Width and height of the texture in pixels

void main()
{
    // Calculate how many times the texture should repeat across the model
    // based on the pixels per unit value
    float texture_repeatX = texture_size.x / texels_per_unit;
    float texture_repeatY = texture_size.y / texels_per_unit;
    
    // Apply the scaling to the texture coordinates
    vec2 scaledTexCoord = vec2(
        fragTexCoord.x / texture_repeatX,
        fragTexCoord.y / texture_repeatY
    );
    
    // Sample the texture with the tiled coordinates
    vec4 texelColor = texture(texture0, fragTexCoord);
    
    finalColor = texelColor;
}