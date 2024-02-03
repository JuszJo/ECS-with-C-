#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../entityV2.h"

class CollisionSystem {
    public:

        CollisionSystem() {}

        bool didCollide(EntityV2 entity1, EntityV2 entity2) {
            if(
                entity1.x + entity1.width > entity2.x &&
                entity1.x < entity2.x + entity2.width &&
                entity1.y + entity1.height > entity2.y &&
                entity1.y < entity2.y + entity2.height
            ) {
                return true;
            }
            else {
                return false;
            }
        }

        void handleCollision(EntityV2* entity1, EntityV2* entity2) {
            if(entity1 -> name == (char*)"bullet" && entity2 -> name == (char*)"enemy") {
                entity1 -> performAction((char*)"remove_bullet");
                entity2 -> performAction((char*)"despawn");
                EnemyManager::reduceEnemyCount();
            }
            if(entity1 -> name == (char*)"bullet" && entity2 -> name == (char*)"player") {
                entity1 -> performAction((char*)"remove_bullet");
                entity2 -> performAction((char*)"despawn");
            }
        }

        void checkCollision() {
            for(int i = 0; i < entityManager.entity_list.size(); ++i) {
                for(int j = 0; j < entityManager.entity_list.size(); ++j) {
                    if(entityManager.entity_list[i]->active && entityManager.entity_list[j]->active) {
                        if(entityManager.entity_list[i] != entityManager.entity_list[j]) {
                            if(didCollide(*entityManager.entity_list[i], *entityManager.entity_list[j])) {
                                std::cout << entityManager.entity_list[i]->name << " colliding with " << entityManager.entity_list[j]->name << std::endl;
                                handleCollision(entityManager.entity_list[i], entityManager.entity_list[j]);
                            }
                        }
                    }
                }
            }
        }

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