#version 330 core

in vec2 textureCoord;
in vec3 bulletColor;

uniform sampler2D myTexture;

out vec4 FragColor;

void main() {
    FragColor = texture(myTexture, textureCoord) * vec3(bulletColor, 1.0);
}