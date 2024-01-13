#ifndef GRAVITY_H
#define GRAVITY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Gravity {
    public:
        float gForce = 0.5f;
        bool apply = true;

        Gravity() {}

        void applyGravity(glm::vec3* speed) {
            if(apply) {
                // printf("applying\n");
                speed -> y -= gForce;
            }
        }

        void stopGravity() {
            apply = false;
        }

        void startGravity() {
            apply = true;
        }
};

#endif