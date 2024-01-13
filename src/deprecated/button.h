#ifndef BUTTON_H
#define BUTTON_H

#include "entity.h"

class Button: public Entity {
    private:
        int stride = 5;

    public:
        float buttonWidth, buttonHeight, buttonX, buttonY;
        bool active = false;
        // const char* axis = "origin";
        const char* axis = "center";
        char* name;

        // default constructor
        Button() {}

        Button(char* texturePath, float width, float height, float x, float y, char* buttonName) {
            buttonWidth = width;
            buttonHeight = height;
            buttonX = x;
            buttonY = y;
            name = buttonName;

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

        void updatePosition(float newButtonX, float newButtonY) {
            buttonX = newButtonX;
            buttonY = newButtonY;
        }

        void updateSize(float newButtonWidth, float newButtonHeight) {
            buttonWidth = newButtonWidth;
            buttonHeight = newButtonHeight;
        }

        void setPosition(float newButtonX, float newButtonY) {
            model = glm::translate(model, glm::vec3(newButtonX, newButtonY, 0.0f));

            model = glm::translate(model, glm::vec3(-buttonX, -buttonY, 0.0f));

            updatePosition(newButtonX, newButtonY);
        }

        void experimentalSizeUpdate(float newWidth, float newHeight) {
            float dWidth = buttonWidth - newWidth;
            float dHeight = buttonHeight - newHeight;

            if(axis == (char*)"center") {
                model = glm::mat4(1.0f);

                float newButtonX = buttonX + (dWidth / 2.0f);
                float newButtonY = buttonY + (dHeight / 2.0f);

                // printf("current buttonX: %f, new buttonX: %f\n", buttonX, newButtonX);

                float vertices[20] = {
                    newButtonX, newButtonY, 0.0f, 0.0f, 1.0f,
                    newButtonX + newWidth, newButtonY, 0.0f, 1.0f, 1.0f,
                    newButtonX, newButtonY + newHeight, 0.0f, 0.0f, 0.0f,
                    newButtonX + newWidth, newButtonY + newHeight, 0.0f, 1.0f, 0.0f
                };

                genVertexandBuffers(&VAO, &VBO);
                bindVAO(VAO);

                int verticeSize = sizeof(vertices);
                handleVertexBufferObject(VBO, vertices, verticeSize);

                buttonX = newButtonX;
                buttonY = newButtonY;
            }
            else {
                float vertices[20] = {
                    buttonX, buttonY, 0.0f, 0.0f, 1.0f,
                    buttonX + newWidth, buttonY, 0.0f, 1.0f, 1.0f,
                    buttonX, buttonY + newHeight, 0.0f, 0.0f, 0.0f,
                    buttonX + newWidth, buttonY + newHeight, 0.0f, 1.0f, 0.0f
                };

                genVertexandBuffers(&VAO, &VBO);
                bindVAO(VAO);

                int verticeSize = sizeof(vertices);
                handleVertexBufferObject(VBO, vertices, verticeSize);
            }

            buttonWidth = newWidth;
            buttonHeight = newHeight;

            handleVertexArrayObject(0, 3, stride, 0);
            handleVertexArrayObject(1, 2, stride, 3);

            cleanupBuffers();
        }

        void processInput(GLFWwindow* window){
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                // printf("OMOMOMOMOM\n");
                std::cout << model[3][0] << std::endl;
                // std::cout << model[1][1] << std::endl;
                model = glm::scale(model, glm::vec3(1.001f, 1.001f, 1.0f));
            }
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
                // printf("OMOMOMOMOM\n");
                model = glm::translate(model, glm::vec3(1.01f, 0.0f, 0.0f));
                buttonX += 1.01f;
            }
        }

        void scale(float scaleFactorX, float scaleFactorY) {
            float scaledWidth = buttonWidth * scaleFactorX;
            float scaledHeight = buttonHeight * scaleFactorY;

            if(axis == (char*)"center") {
                float dWidth = buttonWidth - scaledWidth;
                float dHeight = buttonHeight - scaledHeight;

                float newButtonX = buttonX + (dWidth / 2);
                float newButtonY = buttonY + (dHeight / 2);

                model = glm::translate(model, glm::vec3(buttonX + (buttonWidth / 2), buttonY + (buttonHeight / 2), 0.0f));

                model = glm::scale(model, glm::vec3(scaleFactorX, scaleFactorY, 1.0f));

                model = glm::translate(model, glm::vec3(-(buttonX + (buttonWidth / 2)), -(buttonY + (buttonHeight / 2)), 0.0f));

                updatePosition(newButtonX, newButtonY);
            }
            else {
                model = glm::translate(model, glm::vec3(buttonX, buttonY, 0.0f));

                model = glm::scale(model, glm::vec3(scaleFactorX, scaleFactorY, 1.0f));

                model = glm::translate(model, glm::vec3(-buttonX, -buttonY, 0.0f));
            }
            updateSize(scaledWidth, scaledHeight);
        }

        void render() {
            glBindTexture(GL_TEXTURE_2D, TBO);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
};

#endif