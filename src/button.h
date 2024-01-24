#ifndef BUTTON_H
#define BUTTON_H

class Button {
    private:
        int stride = 5;
        void (*action)();

    public:
        Shader* myShader;
        
        unsigned int VAO, VBO, EBO, TBO;

        float x, y, width, height;

        glm::mat4 model = glm::mat4(1.0f);

        bool mousePressed = false;

        Button(Shader* shader, char* path, float position_x, float position_y, float size_width, float size_height, void (*button_action)()) {
            myShader = shader;

            x = position_x;
            y = position_y;
            width = size_width;
            height = size_height;
            
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

            loadImage(path, &TBO);

            action = button_action;
        }

        void updatePosition(float newX, float newY) {
            x = newX;
            y = newY;
        }

        void updateSize(float newWidth, float newHeight) {
            width = newWidth;
            height = newHeight;
        }

        void scale(float scaleX, float scaleY) {
            float scaledWidth = width * scaleX;
            float scaledHeight = height * scaleY;

            float dWidth = width - scaledWidth;
            float dHeight = height - scaledHeight;

            float newX = x + (dWidth / 2);
            float newY = y + (dHeight / 2);
            // float newX = x;
            // float newY = y;

            // std::cout << "newX: " << newX << "\t" << "newY: " << newY << std::endl;

            model = glm::mat4(1.0f);

            model = glm::translate(model, glm::vec3(newX, newY, 0.0f));

            model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.0f));
        }

        virtual void setPosition(float newX, float newY) {
            model = glm::translate(model, glm::vec3(newX, newY, 0.0f));

            model = glm::translate(model, glm::vec3(-x, -y, 0.0f));

            updatePosition(newX, newY);
        }

        virtual void resetModel() {
            model = glm::mat4(1.0f);
        }

        void setUniformVec3f(Shader* shader, char* name, float val1, float val2, float val3) {
            int colorLocation = glGetUniformLocation(shader -> shaderProgram, name);
            glUniform3f(colorLocation, val1, val2, val3);
        }

        void setUniform1f(Shader* shader, char* name, float value) {
            int location = glGetUniformLocation(shader -> shaderProgram, name);
            glUniform1f(location, value);
        }

        void setUniform1i(Shader* shader, char* name, int value) {
            int location = glGetUniformLocation(shader -> shaderProgram, name);
            glUniform1f(location, value);
        }

        void setUniformMatrix4fv(Shader* shader, char* name, glm::mat4 value) {
            int location = glGetUniformLocation(shader -> shaderProgram, name);
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }

        bool checkHover() {
            if(
                cursor_position_x < x + width &&
                cursor_position_x > x &&
                abs(cursor_position_y - display_h) < y + height &&
                abs(cursor_position_y - display_h) > y
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

        void runAction() {
            action();
        }

        void update() {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x, y, 0.0f));
            if(checkHover()) scale(2.0f, 2.0f);

            if(checkHover() && mousePressed) runAction();
        }

        void render(glm::mat4 projection) {
            // printf("LOL\n");
            myShader -> use();
            glUniformMatrix4fv(glGetUniformLocation(myShader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(myShader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindTexture(GL_TEXTURE_2D, TBO);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

    protected:
        void genVertexandBuffers(unsigned int* VAO, unsigned int* VBO) {
            glGenVertexArrays(1, VAO);
            glGenBuffers(1, VBO);
        }

        void genElementBuffers(unsigned int* EBO) {
            glGenBuffers(1, EBO);
        }

        void bindVAO(unsigned int VAO) {
            glBindVertexArray(VAO);
        }

        void handleVertexBufferObject(unsigned int VBO, float* vertices, float size) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        }

        void handleElementBuffer(unsigned int EBO, unsigned int* indices, float size) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_DYNAMIC_DRAW);
        }

        void handleVertexArrayObject(int location, int nComponents, int stride, int offset) {
            glVertexAttribPointer(location, nComponents, GL_FLOAT, GL_FALSE, stride * sizeof(float), offset == 0 ? nullptr : (void*)(offset * sizeof(float)));
            glEnableVertexAttribArray(location);
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

        void cleanupBuffers() {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
};

#endif