#version 400

// input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// output fragment color
out vec4 finalColor;

void main()
{
    // sample the texture
    vec4 texelColor = texture(texture0, fragTexCoord);
    
    // apply tint and color - fully lit (no lighting calculations)
    finalColor = texelColor * colDiffuse * fragColor;
}