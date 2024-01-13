#ifndef MENU_H
#define MENU_H

#include "../libs/shader.h"

#include "button.h"

class Menu {
    private:
        void startGame() {
            gameStart = true;
            setDisplay(false);
        }

    protected:
        void quit() {
            printf("QUIT\n");
        }

    public:
        unsigned int VAO, VBO, TBO;

        glm::mat4 model = glm::mat4(1.0f);
        float menuWidth = 800.0f;
        float menuHeight = 600.0f;

        bool display = false;
        int* display_w;
        int* display_h;
        double cursor_position_x, cursor_position_y;
        bool mousePressed = false;
        char* name = (char*)"Main Menu";

        Button buttons[4];
        int currentButtonIndex = 0;

        Menu(int* display_width, int* display_height, char* path = nullptr) {
            display_w = display_width;
            display_h = display_height;

            float vertices[] = {
                0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.0f + 800.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                0.0f, 0.0f + 600.0f, 0.0f, 0.0f, 0.0f,
                0.0f + 800.0f, 0.0f + 600.0f, 0.0f, 1.0f, 0.0f
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

            if(path != nullptr) loadImage(path, &TBO);
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

        void experimentalSizeUpdate(float newWidth, float newHeight) {
            model = glm::mat4(1.0f);

            menuWidth = newWidth;
            menuHeight = newHeight;

            float vertices[] = {
                0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.0f + newWidth, 0.0f, 0.0f, 1.0f, 1.0f,
                0.0f, 0.0f + newHeight, 0.0f, 0.0f, 0.0f,
                0.0f + newWidth, 0.0f + newHeight, 0.0f, 1.0f, 0.0f
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

        void updateMenuItems(float scaleFactorX, float scaleFactorY, GLFWwindow* window) {
            for(int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); ++i) {
                if(buttons[i].active) {
                    // buttons[i].processInput(window);
                    float dWidth = buttons[i].buttonWidth - buttons[i].buttonWidth * scaleFactorX;
                    float dHeight = buttons[i].buttonHeight - buttons[i].buttonHeight * scaleFactorY;

                    buttons[i].setPosition((buttons[i].buttonX * scaleFactorX) - (dWidth / 2.0f), (buttons[i].buttonY * scaleFactorY) - (dHeight / 2.0f));
                    // buttons[i].experimentalSizeUpdate(buttons[i].buttonWidth * scaleFactorX, buttons[i].buttonHeight * scaleFactorY);
                }
            }
        }

        void experimentalScale(float scaleFactorX, float scaleFactorY) {
            model = glm::scale(model, glm::vec3(scaleFactorX, scaleFactorY, 1.0f));

            menuWidth *= scaleFactorX;
            menuHeight *= scaleFactorY;
        }

        void setDisplay(bool value) {
            display = value;
        }

        void addButton(char* texturePath, float width, float height, float x, float y, char* buttonName) {
            Button newButton(texturePath, width, height, x, y, buttonName);

            newButton.active = true;

            buttons[currentButtonIndex] = newButton;

            ++currentButtonIndex;
        }

        void updateMenuCursor(GLFWwindow* window) {
            glfwGetCursorPos(window, &cursor_position_x, &cursor_position_y);
        }

        bool checkHover(Button currentButton) {
            if(
                cursor_position_x < currentButton.buttonX + currentButton.buttonWidth &&
                cursor_position_x > currentButton.buttonX &&
                abs(cursor_position_y - *display_h) < currentButton.buttonY + currentButton.buttonHeight &&
                abs(cursor_position_y - *display_h) > currentButton.buttonY
            ) {
                return true;
            }
            else {
                return false;
            }
        }

        void checkMousePress(GLFWwindow* window) {
            if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                mousePressed = true;
            }
            else {
                mousePressed = false;
            }
        }

        virtual void runAction(Button currentButton) {
            if(currentButton.name == (char*)"play") {
                startGame();
            }
            if(currentButton.name == (char*)"quit") {
                quit();
            }
        }

        void render(GLFWwindow* window, Shader* menuShader, glm::mat4 projection) {
            glUniformMatrix4fv(glGetUniformLocation(menuShader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(menuShader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindTexture(GL_TEXTURE_2D, TBO);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            // std::cout << name << std::endl;
            // std::cout << mousePressed << std::endl;
            // std::cout << cursor_position_x << "\t" << abs(cursor_position_y - (double)600.0) << std::endl;

            for(int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); ++i) {
                checkMousePress(window);

                Button currentButton = buttons[i];

                if(currentButton.active) {
                    // if(checkHover(currentButton)) currentButton.scale(2.0f, 2.0f);
                    if(checkHover(currentButton)) currentButton.experimentalSizeUpdate(currentButton.buttonWidth * 2.0f, currentButton.buttonHeight * 2.0f);
                    if(checkHover(currentButton) && mousePressed) {
                        runAction(currentButton);
                    }
                    currentButton.setProjection(menuShader, projection);
                    currentButton.setUniformMatrix4fv(menuShader, (char*)"model");
                    currentButton.render();
                }
            }
        }

        void stopGame() {
            gameStart = false;
            setDisplay(true);
        }

};

#endif