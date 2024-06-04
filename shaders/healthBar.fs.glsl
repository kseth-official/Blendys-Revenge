#version 330 core

in vec2 fragTexCoord;
out vec4 fragColor;

uniform vec3 barColor; // Color of the health bar
uniform float healthPercentage; // Health percentage in the range [0.0, 1.0]

void main()
{
     // Set the health bar color to a custom color (e.g., green)
    vec3 customColor = vec3(0.0, 1.0, 0.0);

    // Mix the custom color with the original barColor based on healthPercentage
    vec3 color = mix(customColor, barColor, healthPercentage);

    //adjust healthPercentage to partially show the health bar
    float partialHealth = min(1.0, max(0.0, healthPercentage - 0.25));

    // Check if the fragment is outside the health bar range
    if (fragTexCoord.x > partialHealth) {
        // Discard the fragment (don't render it)
        discard;
    }

    // Render the health bar
    fragColor = vec4(color, 1.0);
}
