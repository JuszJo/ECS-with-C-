#version 330 core

in vec2 textureCoord;

uniform sampler2D myTexture;
uniform vec3 bulletColor;

out vec4 FragColor;

void main() {
    // FragColor = texture(myTexture, textureCoord) * vec4(bulletColor, 1.0);

    // Sample the texture at the specified coordinates and convert to grayscale
    vec4 color = texture(myTexture, textureCoord);

    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    color = vec4(vec3(gray), color.a);

    // Apply the color tint
    FragColor = color * vec4(bulletColor, 1.0);
}