#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "entityV2.h"

class EntityManager {
    public:
        EntityV2* entityList[1];
        int currentIndex = 0;

        EntityManager() {}

        void addEntity(EntityV2* entity) {
            entityList[currentIndex] = entity;

            ++currentIndex;
        }

        EntityV2* getEntity(char* name) {
            for(int i = 0; i < sizeof(entityList) / sizeof(entityList[0]); ++i) {
                if(entityList[i] -> name == name) {
                    return entityList[i];
                }
            }

            return NULL;
        }

        /* EntityV2** getEntities(char** arr, int arraySize) {
            int foundEntities = 0;

            for(int i = 0; i < sizeof(entityList) / sizeof(entityList[0]); ++i) {
                for(int j = 0; j < arraySize; ++j) {
                    if(entityList[i] -> name == arr[i]) {

                        ++foundEntities;
                    }
                }
            }

            EntityV2** entities = (EntityV2**)malloc(foundEntities * sizeof(EntityV2));
            int entityIndex = 0;

            for(int i = 0; i < sizeof(entityList) / sizeof(entityList[0]); ++i) {
                for(int j = 0; j < arraySize; ++j) {
                    if(entityList[i] -> name == arr[i]) {

                        entities[entityIndex] = entityList[i];
                        ++entityIndex;
                    }
                }
            }

            return entities;
        } */
};

#endif