#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../entityV2.h"

class CollisionSystem {
    public:
        int currentIndex = 0;

        CollisionSystem() {}

        /* void checkCollision() {
            for(int i = 0; i < size; ++i) {
                for(int j = 0; j < size; ++j) {
                    if(entityList[i] -> active && entityList[j] -> active) {
                        if(entityList[i] != entityList[j]) {
                            if(didCollide(*entityList[i], *entityList[j])) {
                                std::cout << entityList[i] -> name << " colliding with " << entityList[j] -> name << std::endl;
                                gameActions(entityList[i], entityList[j]);
                            }
                        }
                    }
                }
            }
        } */
        void checkCollision() {
            for(int i = 0; i < entityManager.entity_list.size(); ++i) {
                for(int j = 0; j < entityManager.entity_list.size(); ++j) {
                    if(entityManager.entity_list[i]->active && entityManager.entity_list[j]->active) {
                        if(entityManager.entity_list[i] != entityManager.entity_list[j]) {
                            if(didCollide(*entityManager.entity_list[i], *entityManager.entity_list[j])) {
                                std::cout << entityManager.entity_list[i]->name << " colliding with " << entityManager.entity_list[j]->name << std::endl;
                                gameActions(entityManager.entity_list[i], entityManager.entity_list[j]);
                            }
                        }
                    }
                }
            }
        }

        /* void wallCollision() {
            for(int i = 0; i < entityManager.entity_list.size(); ++i) {
                if(entityList[i] -> active) {
                    if(entityList[i] -> x < 0.0f) {
                        entityList[i] -> setPosition(0.0f, entityList[i] -> y);
                    }
                    if(entityList[i] -> x + entityList[i] -> width > 800.0f) {
                        entityList[i] -> setPosition(800.0f - entityList[i] -> width, entityList[i] -> y);
                    }
                }
            }
        } */
        void wallCollision() {
            for(int i = 0; i < entityManager.entity_list.size(); ++i) {
                if(entityManager.entity_list[i]->active) {
                    if(entityManager.entity_list[i]->x < 0.0f) {
                        entityManager.entity_list[i]->setPosition(0.0f, entityManager.entity_list[i]->y);
                    }
                    if(entityManager.entity_list[i]->x + entityManager.entity_list[i]->width > 800.0f) {
                        entityManager.entity_list[i]->setPosition(800.0f - entityManager.entity_list[i]->width, entityManager.entity_list[i]->y);
                    }
                }
            }
        }
};

#endif