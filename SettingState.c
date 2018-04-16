#include "SettingState.h"

char *diff[5] = {"amateur", "easy", "moderate", "hard", "expert"};


//Settings Functions: ------------------------------------------------------------------------


/* The function Handles the game's settings initializations.
 * It asks from the user for settings and set it.
 * It returns 0 if the user want to start the game and 1 if he
 * want to quit the game.
 */
int SettingState(){
    int ParseResult;
    InitMovesHistory();
    printf(START_SETTING);
    while(1){
        GetInput();
        ParseResult = ParsingInput();
        if (ParseResult == 0 || ParseResult == 1) return ParseResult;
    }
}


/* This function is an auxiliary function for SettingState.
 * It asks for input from the user and set the appropriate setting:
 * game mode, difficulty, user color, load, default, print settings, quit and start.
 * It returns -1 if the settings initialization not finished,
 * 0 if the user ask to start the game, and 1 if he want to exit.
 */
int ParsingInput(){
    char *Cmd;
    char *Arg;
    Cmd = strtok(Input ," ");
    Arg = strtok(NULL, " ");
    // empty command
    if (Cmd == NULL){
        printf(INVALID_CMD);
    }
    //handling the setting commands
    else if (strcmp(Cmd, "game_mode") == 0){
        if(Arg == NULL || strlen(Arg) != 1){
            printf(GAME_MODE_ERROR);
        }
        else SetGameMode(Arg[0]);
    }
    else if(strcmp(Cmd , "difficulty") == 0){
        if(Arg == NULL || atoi(Arg) == 0){
            printf(INVALID_CMD);
        }
        else SetDifficulty(Arg);
    }
    else if (strcmp(Cmd, "user_color") == 0) {
        if (Arg == NULL || strlen(Arg) != 1) {
            printf(WRONG_COLOR_CMD);
        } else SetColor(Arg[0]);
    }
    else if (strcmp(Cmd, "load") == 0) {
        if(Arg == NULL){
            printf(INVALID_CMD);
            return -1;
        }
        if(!LoadPrevGame(Arg)){
           printf(LOAD_ERR);
            return -1;
        }
        else{
            printf(START_CMD);
            return 0;
        }
    }
    else if (strcmp(Cmd , "default") == 0){
        SetDefault();
        printf(DEFAULT_SETTING_CMD);
    }
    else if (strcmp(Cmd, "print_settings") == 0) {
        PrintSettings(GameMode);
    }
    else if (strcmp(Cmd, "quit") == 0) {
        printf(EXIT_CMD);
        return 1;
    }
    else if (strcmp(Cmd, "start") == 0) {
        printf(START_CMD);
        return 0;
    }
    else{
        printf(INVALID_CMD);
    }
    return -1;
}

/*The function gets a char. If the char is valid (1 or 2)
 * it sets the game mode to be for 1 player or 2 players, respectively.
 * Otherwise, it prints an error message.
 */
void SetGameMode(char c){
    if (c == '1') {
        GameMode = 1;
        printf("Game mode is set to 1-player\n");
    } else if (c == '2') {
        GameMode = 2;
        printf("Game mode is set to 2-player\n");
    } else printf(GAME_MODE_ERROR);
}

/*The function gets a string. If the string is valid (a number between 1 to 5)
 * it sets the difficulty to be that number(just if its 1 player mode).
 * Otherwise, it prints an error message.
 */
void SetDifficulty(char *c){
    int num;
    num = atoi(c);
    if(GameMode == 2) printf(INVALID_CMD);
    else if(num <= 5 && num>= 1){
        Difficulty = num;
        printf("Difficulty level is set to %s\n", diff[atoi(c)-1]);
    }
    else printf(WRONG_DIFF_CMD);
}

/*The function gets a char. If the char is valid (0 or 1)
 * it sets the player's color to be black or white , respectively.
 * Otherwise, it prints an error message.
 */
void SetColor(char c){
    if (GameMode == 2) printf(INVALID_CMD);
    else if (c == '0' || c == '1'){
        UserColor = (c == '0' ? BLACK : WHITE);
        CompColor = (c == '0' ? WHITE : BLACK);
        printf("User color is set to %s\n", UserColor);
    }
    else printf(WRONG_COLOR_CMD);
}

/*The function gets a number - the game mode,
 * and prints the settings that were initialized.
 */
void PrintSettings(int GameMode){
    if (GameMode == 2){
        printf("SETTINGS:\nGAME_MODE: 2-player\n");
    }
    else {
        printf("SETTINGS:\nGAME_MODE: 1-player\nDIFFICULTY: %s\nUSER_COLOR: %s\n" , diff[Difficulty-1] , UserColor);
    }
}