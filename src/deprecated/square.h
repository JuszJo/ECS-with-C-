#ifndef SQUARE_H
#define SQUARE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity.h"

class Square: public Entity {
    private:
        int stride = 5;

    public:
        float squareWidth, squareHeight, squareX, squareY;
        const char* axis = "origin";

        bool shouldAnimate = false;

        float totalFrames = 1.0f;
        float currentFrame = 1.0f;

        int animateBuffer = 1000;
        int elapsedFrames = 0;

        // default constructor
        Square() {}

        Square(char* texturePath, float width, float height, float x, float y) {
            squareWidth = width;
            squareHeight = height;
            squareX = x;
            squareY = y;

            float vertices[20] = {
                x, y, 0.0f, 0.0f, 1.0f,
                x + width, y, 0.0f, 1.0f, 1.0f,
                x, y + height, 0.0f, 0.0f, 0.0f,
                x + width, y + height, 0.0f, 1.0f, 0.0f
            };
            genVertexandBuffers(&VAO, &VBO);
            bindVAO(VAO);

            int verticeSize = sizeof(vertices);
            handleVertexBufferObject(VBO, vertices, verticeSize);

            handleVertexArrayObject(0, 3, stride, 0);
            handleVertexArrayObject(1, 2, stride, 3);

            cleanupBuffers();

            loadImage(texturePath);
        }

        void updatePosition(float newSquareX, float newSquareY) {
            squareX = newSquareX;
            squareY = newSquareY;
        }

        void updateSize(float newSquareWidth, float newSquareHeight) {
            squareWidth = newSquareWidth;
            squareHeight = newSquareHeight;
        }

        void setPosition(float newSquareX, float newSquareY) {
            model = glm::translate(model, glm::vec3(newSquareX, newSquareY, 0.0f));

            model = glm::translate(model, glm::vec3(-squareX, -squareY, 0.0f));

            updatePosition(newSquareX, newSquareY);
        }

        void scale(float scaleFactorX, float scaleFactorY) {
            
            model = glm::scale(model, glm::vec3(scaleFactorX, scaleFactorY, 1.0f));

            // float scaledWidth = squareWidth * scaleFactorX;
            // float scaledHeight = squareHeight * scaleFactorY;

            // if(axis == (char*)"center") {
            //     float dWidth = scaledWidth - squareWidth;
            //     float dHeight = scaledHeight - squareHeight;

            //     float newSquareX = squareX - (dWidth / 2);
            //     float newSquareY = squareY - (dHeight / 2);

            //     model = glm::translate(model, glm::vec3(squareX + (squareWidth / 2), squareY + (squareHeight / 2), 0.0f));

            //     model = glm::scale(model, glm::vec3(scaleFactorX, scaleFactorY, 1.0f));

            //     model = glm::translate(model, glm::vec3(-(squareX + (squareWidth / 2)), -(squareY + (squareHeight / 2)), 0.0f));

            //     updatePosition(newSquareX, newSquareY);
            // }
            // else {
            //     model = glm::translate(model, glm::vec3(squareX, squareY, 0.0f));

            //     model = glm::scale(model, glm::vec3(scaleFactorX, scaleFactorY, 1.0f));

            //     model = glm::translate(model, glm::vec3(-squareX, -squareY, 0.0f));

            //     // std::cout << squareX << "\t" << squareY << std::endl;
            //     // std::cout << scaleFactorX << "\t" << scaleFactorY << std::endl;
            // }
            // updateSize(scaledWidth, scaledHeight);
        }

        void animate(Shader* shader) {
            if(shouldAnimate) {
                if(elapsedFrames % animateBuffer == 0) {
                    if(currentFrame > totalFrames) currentFrame = 1;

                    setUniform1f(shader, (char*)"totalFrames", totalFrames);
                    setUniform1f(shader, (char*)"currentFrame", currentFrame);

                    ++currentFrame;

                    elapsedFrames = 0;
                }

                ++elapsedFrames;
            }
            else {
                currentFrame = 1;
                elapsedFrames = 0;

                setUniform1f(shader, (char*)"totalFrames", totalFrames);
                setUniform1f(shader, (char*)"currentFrame", currentFrame);
            }
        }

        void render() {
            glBindTexture(GL_TEXTURE_2D, TBO);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
};

#endif