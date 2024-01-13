#ifndef PLAYER_MENU_H
#define PLAYER_MENU_H

#include "menu.h"
#include "player.h"

class PlayerMenu: public Menu {
    public:
        Menu* mainMenu;
        Player* player;


        PlayerMenu(Menu* menu, Player* owner, int* display_w, int* display_h): Menu(display_w, display_h) {
            mainMenu = menu;
            player = owner;
            name = (char*)"Player Menu";
        }

        void processInput(GLFWwindow* window) {
            if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                pause();
            }
        }

        void pause() {
            setDisplay(true);
        }

        void quitToMainMenu() {
            player -> resetPlayer();

            setDisplay(false);

            mainMenu -> stopGame();
        }

        void runAction(Button currentButton) override {
            // std::cout << "RUNNING ACTION IN:" << name << std::endl;

            if(currentButton.name == (char*)"play") {
                // std::cout << "PLAY CLICKED IN:" << name << std::endl;
                setDisplay(false);
            }
            if(currentButton.name == (char*)"quit") {
                // std::cout << "QUIT CLICKED IN:" << name << std::endl;
                quitToMainMenu();
            }
        }
};

#endif