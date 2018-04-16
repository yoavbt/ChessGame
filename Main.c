#include "Main.h"

int main(int argc , char* argv[]){
    if ((argv[1] != NULL) && (strcmp(argv[1], "-g") == 0)) {
        Gui(); // Start the Gui Mode
    } else {
        Console(); //Start the console mode
    }
    return 1;
}

/**
 * param: none
 * return : void
 * action : start the console mode loop
 *
 */
void Console(){
    int SettingStateResult, GameStateResult;
    // print the first line of the game
    printf(" Chess\n");
    printf("-------\n");
    InitBoard(); // initialize the board
    SetDefault(); // set all settings to default
    while(1) {
        SettingStateResult = SettingState(); // 1 if exit, 0 if start
        if (SettingStateResult) exit(EXIT_SUCCESS);
        GameStateResult = GameState();// 1 if quit or end of game, 2 if Memory allocation error ,3 for reset
        if (GameStateResult == 2)exit(EXIT_FAILURE);
        else if (GameStateResult == 1)exit(EXIT_SUCCESS);
    }
}

/**
 * param: none
 * return : void
 * action : start the Gui mode main functions
 *
 */

void Gui(){
    InitGui();
    StartGui();
    QuitGui();
}
