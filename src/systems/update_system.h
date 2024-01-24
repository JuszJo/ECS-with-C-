#ifndef UPDATE_SYSTEM_H
#define UPDATE_SYSTEM_H

#include "../entityV2.h"

class UpdateSystem {
    public:
        int currentIndex = 0;

        UpdateSystem() {}

        /* void update() {
            // std::cout << "size in update:" << arraySize << std::endl;
            for(int i = 0; i < size; ++i) {
                if(entityList[i] -> active) {
                    entityList[i] -> update();
                }
            }
        } */
        void update() {
            // std::cout << "size in update:" << arraySize << std::endl;
            for(int i = 0; i < entityManager.entity_list.size(); ++i) {
                if(entityManager.entity_list[i]->active) {
                    entityManager.entity_list[i]->update();
                }
            }
        }
};

#endif