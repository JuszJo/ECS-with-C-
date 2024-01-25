#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../entityV2.h"

class RenderSystem {
    public:

        RenderSystem() {}

        void render(glm::mat4 projection, glm::mat4 view) {
            // std::cout << arraySize << std::endl;
            for(int i = 0; i < entityManager.entity_list.size(); ++i) {
                if(entityManager.entity_list[i]->active) {
                    entityManager.entity_list[i]->shader -> use();
                    entityManager.entity_list[i]->render(projection, view);
                }
            }
        }
};

#endif