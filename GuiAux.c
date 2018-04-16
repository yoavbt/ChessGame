#include "GuiAux.h"

/**
 * This Model is for defining QUIT functions from the SDL
 * and also contain Macro constants and extern variables
 *
 */

/**
 * param: none
 * return: void
 * action: exit SDL with success code
 */

void QuitGui() {
    SDL_Quit();
    exit(EXIT_SUCCESS);
};

/**
 * param: none
 * return: void
 * action: exit SDL with fail code
 */

void QuitFailureGui() {
    SDL_Quit();
    exit(EXIT_FAILURE);
}
