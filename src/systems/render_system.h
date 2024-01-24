#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../entityV2.h"

class RenderSystem {
    public:
        int currentIndex = 0;

        RenderSystem() {}

        /* void render(Shader* shader, glm::mat4 projection, glm::mat4 view) {
            // std::cout << arraySize << std::endl;
            for(int i = 0; i < size; ++i) {
                if(entityList[i] -> active) {
                    entityList[i] -> shader -> use();
                    entityList[i] -> render(shader, projection, view);
                }
            }
        } */
        void render(Shader* shader, glm::mat4 projection, glm::mat4 view) {
            // std::cout << arraySize << std::endl;
            for(int i = 0; i < entityManager.entity_list.size(); ++i) {
                if(entityManager.entity_list[i]->active) {
                    entityManager.entity_list[i]->shader -> use();
                    entityManager.entity_list[i]->render(shader, projection, view);
                }
            }
        }
};

#endif