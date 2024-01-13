#version 330 core

in vec2 textureCoord;

uniform sampler2D myTexture;
uniform float totalFrames;
uniform float currentFrame;

out vec4 FragColor;

void main() {
    float frameWidth = 1.0 / totalFrames;

    vec2 FrameCoord = textureCoord * vec2(frameWidth, 1.0);

    FrameCoord = vec2(FrameCoord.x + ((currentFrame - 1.0) * (frameWidth)), FrameCoord.y);

    FragColor = texture(myTexture, FrameCoord);
}