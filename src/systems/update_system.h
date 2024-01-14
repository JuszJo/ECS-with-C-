#ifndef UPDATE_SYSTEM_H
#define UPDATE_SYSTEM_H

#include "../entityV2.h"

class UpdateSystem {
    public:
        int currentIndex = 0;

        UpdateSystem() {}

        void update(EntityV2* entityPointer[], int arraySize) {
            for(int i = 0; i < arraySize; ++i) {
                if(entityPointer[i] -> active) {
                    entityPointer[i] -> update();
                }
            }
        }
};

#endif