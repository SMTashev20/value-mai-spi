#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec2 uv = fragTexCoord;

    // NOTE: Implement here your fragment shader code

    // finalColor = vec4(texelColor.xyz, 1.0);
    finalColor = texelColor * vec4(sin(uv.x), sin(uv.y), cos(uv.x), 1.0);
}

