#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../entityV2.h"

class CollisionSystem {
    public:
        int currentIndex = 0;

        CollisionSystem() {}

        void wallCollision(EntityV2* entityPointer[], int arraySize) {
            for(int i = 0; i < arraySize; ++i) {
                if(entityPointer[i] -> active) {
                    entityPointer[i] -> update();
                }
            }
        }
};

#endif