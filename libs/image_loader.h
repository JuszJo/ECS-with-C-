#ifndef IMAGE_LOADER
#define IMAGE_LOADER

#include <GL/glew.h>

// #ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class ImageLoader {
    void loadImage(unsigned int* TBO) {
        int width, height, nChannels;

        stbi_uc* imageData = stbi_load("src/assets/playbutton.png", &width, &height, &nChannels, 0);

        glGenTextures(1, TBO);
        glBindTexture(GL_TEXTURE_2D, *TBO);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(imageData);
    }
};

#endif