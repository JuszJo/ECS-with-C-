#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../entityV2.h"

class RenderSystem {
    public:
        int currentIndex = 0;

        RenderSystem() {}

        void render(EntityV2* entityPointer[], int arraySize, Shader* shader, glm::mat4 projection, glm::mat4 view) {
            // std::cout << arraySize << std::endl;
            for(int i = 0; i < arraySize; ++i) {
                if(entityPointer[i] -> active) {
                    entityPointer[i] -> shader -> use();
                    entityPointer[i] -> render(shader, projection, view);
                }
            }
        }
};

#endif