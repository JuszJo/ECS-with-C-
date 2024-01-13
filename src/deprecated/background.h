#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "entity.h"

class BackgroundBeta {
    private: 
        void oldRender(Shader* shader, float x, float y, float width, float height, glm::mat4 projection) {
            std::cout << x << "\t" << y << std::endl;
            std::cout << width << "\t" << height << std::endl;

            float vertices[] = {
                x, y, 0.0f, 0.0f, 1.0f,
                x + width, y, 0.0f, 1.0f, 1.0f,
                x, y + height, 0.0f, 0.0f, 0.0f,
                x + width, y + height, 0.0f, 1.0f, 0.0f
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

    public:
        unsigned int VAO, VBO, TBO;

        float position_x, position_y, bgWidth, bgHeight;

        glm::mat4 model = glm::mat4(1.0f);

        BackgroundBeta(char* path, float x, float y, float width, float height) {
            position_x = x;
            position_y = y;
            bgWidth = width;
            bgHeight = height;

            float vertices[] = {
                x, y, 0.0f, 0.0f, 1.0f,
                x + width, y, 0.0f, 1.0f, 1.0f,
                x, y + height, 0.0f, 0.0f, 0.0f,
                x + width, y + height, 0.0f, 1.0f, 0.0f
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            loadImage(path, &TBO);
        }

        void loadImage(char* path, unsigned int* TBO) {
            int width, height, nChannels;

            stbi_uc* imageData = stbi_load(path, &width, &height, &nChannels, 0);

            if(!imageData) {
                const char* reason = stbi_failure_reason();

                std::cout << reason << std::endl;
            }

            glGenTextures(1, TBO);
            glBindTexture(GL_TEXTURE_2D, *TBO);

            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            if(nChannels > 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
            }

            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(imageData);
        }

        void experimentalScale(float scaleFactorX, float scaleFactorY) {
            // model = glm::translate(model, glm::vec3(backgroundX, backgroundY, 0.0f));

            model = glm::scale(model, glm::vec3(scaleFactorX, scaleFactorY, 1.0f));

            bgWidth *= scaleFactorX;
            bgHeight *= scaleFactorY;

            // model = glm::translate(model, glm::vec3(-backgroundX, -backgroundY, 0.0f));
        }

        
        void render(Shader* shader, glm::mat4 projection, glm::mat4 view) {
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindTexture(GL_TEXTURE_2D, TBO);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
};

// class Background: public Entity {
//     private:
//         int stride = 5;

//     public:
//         float backgroundWidth, backgroundHeight, backgroundX, backgroundY;
//         const char* axis = "origin";
//         // float totalFrames = 2.0f;

//         bool shouldAnimate = false;

//         float totalFrames = 1.0f;
//         int currentFrame = 1;

//         int animateBuffer = 1000;
//         int elapsedFrames = 0;

//         // default constructor
//         Background() {}

//         Background(char* texturePath, float width, float height, float x, float y) {
//             backgroundWidth = width;
//             backgroundHeight = height;
//             backgroundX = x;
//             backgroundY = y;

//             float vertices[20] = {
//                 x, y, 0.0f, 0.0f, 1.0f,
//                 x + width, y, 0.0f, 1.0f, 1.0f,
//                 x, y + height, 0.0f, 0.0f, 0.0f,
//                 x + width, y + height, 0.0f, 1.0f, 0.0f
//             };
//             genVertexandBuffers(&VAO, &VBO);
//             bindVAO(VAO);

//             int verticeSize = sizeof(vertices);
//             handleVertexBufferObject(VBO, vertices, verticeSize);

//             handleVertexArrayObject(0, 3, stride, 0);
//             handleVertexArrayObject(1, 2, stride, 3);

//             cleanupBuffers();

//             loadImage(texturePath);
//         }

//         void updatePosition(float newbackgroundX, float newbackgroundY) {
//             backgroundX = newbackgroundX;
//             backgroundY = newbackgroundY;
//         }

//         void updateSize(float newbackgroundWidth, float newbackgroundHeight) {
//             backgroundWidth = newbackgroundWidth;
//             backgroundHeight = newbackgroundHeight;
//         }

//         void setPosition(float newbackgroundX, float newbackgroundY) {
//             model = glm::translate(model, glm::vec3(newbackgroundX, newbackgroundY, 0.0f));

//             model = glm::translate(model, glm::vec3(-backgroundX, -backgroundY, 0.0f));

//             updatePosition(newbackgroundX, newbackgroundY);
//         }

//         void scale(float scaleFactorX, float scaleFactorY) {
//             float scaledWidth = backgroundWidth * scaleFactorX;
//             float scaledHeight = backgroundHeight * scaleFactorY;

//             if(axis == (char*)"center") {
//                 float dWidth = scaledWidth - backgroundWidth;
//                 float dHeight = scaledHeight - backgroundHeight;

//                 float newbackgroundX = backgroundX - (dWidth / 2);
//                 float newbackgroundY = backgroundY - (dHeight / 2);

//                 model = glm::translate(model, glm::vec3(backgroundX + (backgroundWidth / 2), backgroundY + (backgroundHeight / 2), 0.0f));

//                 model = glm::scale(model, glm::vec3(scaleFactorX, scaleFactorY, 1.0f));

//                 model = glm::translate(model, glm::vec3(-(backgroundX + (backgroundWidth / 2)), -(backgroundY + (backgroundHeight / 2)), 0.0f));

//                 updatePosition(newbackgroundX, newbackgroundY);
//             }
//             else {
//                 model = glm::translate(model, glm::vec3(backgroundX, backgroundY, 0.0f));

//                 model = glm::scale(model, glm::vec3(scaleFactorX, scaleFactorY, 1.0f));

//                 model = glm::translate(model, glm::vec3(-backgroundX, -backgroundY, 0.0f));
//             }
//             updateSize(scaledWidth, scaledHeight);
//         }

//         void animate(Shader* shader) {
//             if(shouldAnimate) {
//                 if(elapsedFrames % animateBuffer == 0) {
//                     if(currentFrame > totalFrames) currentFrame = 1;

//                     setUniform1f(shader, (char*)"totalFrames", totalFrames);
//                     setUniform1f(shader, (char*)"currentFrame", currentFrame);

//                     ++currentFrame;

//                     elapsedFrames = 0;
//                 }

//                 ++elapsedFrames;
//             }
//             else {
//                 currentFrame = 1;
//                 elapsedFrames = 0;

//                 setUniform1f(shader, (char*)"totalFrames", totalFrames);
//                 setUniform1f(shader, (char*)"currentFrame", currentFrame);
//             }
//         }

//         void render(Shader* shader) {
//             setUniform1f(shader, (char*)"totalFrames", totalFrames);
//             setUniform1f(shader, (char*)"currentFrame", currentFrame);
//             glBindTexture(GL_TEXTURE_2D, TBO);
//             glBindVertexArray(VAO);
//             glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//         }
// };

#endif