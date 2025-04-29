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
uniform float pixels_per_unit;  // How many texture pixels per world unit
uniform vec2 texture_size;     // Width and height of the texture in pixels

void main()
{
    // Calculate how many times the texture should repeat across the model
    // based on the pixels per unit value
    float texture_repeatX = texture_size.x / pixels_per_unit;
    float texture_repeatY = texture_size.y / pixels_per_unit;
    
    // Apply the scaling to the texture coordinates
    vec2 scaledTexCoord = vec2(
        fragTexCoord.x / texture_repeatX,
        fragTexCoord.y / texture_repeatY
    );
    
    // Use fract() to create the tiling effect
    // This will repeat the texture once it goes beyond 1.0
    vec2 tiledTexCoord = fract(scaledTexCoord);
    
    // Sample the texture with the tiled coordinates
    vec4 texelColor = texture(texture0, tiledTexCoord);
    
    // Apply basic lighting and texture
    finalColor = texelColor * colDiffuse;
}