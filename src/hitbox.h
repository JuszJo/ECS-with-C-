#ifndef HITBOX_H
#define HITBOX_H

class Hitbox: public Entity {
    private:
    int stride = 7;

    public:
        float position_x, position_y, width, height;
        float x_offset = 0.0f; 
        float y_offset = 0.0f;

        glm::mat4 model = glm::mat4(1.0f);

        Hitbox() {}

        Hitbox(float x, float y, float w, float h) {
            position_x = x;
            position_y = y;
            width = w;
            height = h;
        }

        void defineOffset(float xAxis, float yAxis) {
            x_offset = xAxis;
            y_offset = yAxis;
        }

        void updateAxis(float x, float y) {
            position_x = x;
            position_y = y;
            position_x += x_offset;
            position_y += y_offset;
        }

        void update(float x, float y, float w, float h) {
            position_x = x;
            position_y = y;
            width = w;
            height = h;
        }

        void render(Shader* shader, glm::mat4 projection, glm::mat4 view) {
            if(!settingShowHitbox) return;
            
            float vertices[] = {
                position_x, position_y, 0.0f, 0.3f, 0.3f, 0.3f, 0.9f,
                position_x + width, position_y, 0.0f, 0.3f, 0.3f, 0.3f, 0.9f,
                position_x, position_y + height, 0.0f, 0.3f, 0.3f, 0.3f, 0.9f,
                position_x + width, position_y + height, 0.0f, 0.3f, 0.3f, 0.3f, 0.9f
            };

            genVertexandBuffers(&VAO, &VBO);
            bindVAO(VAO);

            int verticeSize = sizeof(vertices);
            handleVertexBufferObject(VBO, vertices, verticeSize);

            handleVertexArrayObject(0, 3, stride, 0);
            handleVertexArrayObject(1, 4, stride, 3);

            shader -> use();

            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(shader -> shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            // glBindTexture(GL_TEXTURE_2D, *animation.currentAnimatedState[0].TBO);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
};

#endif