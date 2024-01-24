#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "entityV2.h"

class EntityManager {
    public:
        MyArray<EntityV2*> entity_list;

        EntityManager() {}

        void addEntity(EntityV2* entity) {
            entity_list.add_element(entity);
        }

        void removeInactive() {
            int elementsRemoved = 0;

            for(int i = 0; i < entity_list.size(); ++i) {
                if(entity_list[i]->active == false) {
                    entity_list.remove_element(i);
                    
                    --i;
                }
            }
        }
};

#endif