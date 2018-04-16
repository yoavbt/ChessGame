#include "SaveLoadModel.h"

/**----- This Model contain function for load games and save games for the Chess game----
 * -- we assume the load file to be correct
 */


FILE * discFile; // Global File variable use also for load and save


char *diff2[5] = {"amateur", "easy", "moderate", "hard", "expert"};// array for the difficulty level
char LoadBuffer[30]; // Load Buffer Array for the data from file


/**------------ The Save Functions------------
 *
 */


/** param: char* Path -> contain the absolute or relative path of the file name to save to
 *  return: int -> 0 for error or 1 for success saving to file
 *  action: saving the format data to a file in a Path variable
 */

int SaveCurrGame(char * Path) {

    discFile = fopen(Path, "w");
    if (discFile == NULL) {
        return 0;
    }
    SaveSettings();
    SaveBoard();
    if (fclose(discFile)!= 0){
        printf(ERROR, "fclose failed\n");
        return 0;
    }
    return 1;
}

/**
 *  param: none
 *  return: void
 *  action: saving the data format Settings using the fprintf function to the file
 */



void SaveSettings(){

    fprintf(discFile , "%s\n" , CurrPlayerColor);

    if (GameMode == 2) {
        fprintf(discFile,"SETTINGS:\nGAME_MODE: 2-player\n");
    } else {
        fprintf(discFile , "SETTINGS:\nGAME_MODE: 1-player\nDIFFICULTY: %s\nUSER_COLOR: %s\n", diff2[Difficulty-1], UserColor);
    }
}

/**
 *  param: none
 *  return: void
 *  action: saving the data format Board using the fprintf function to the file
 */


void SaveBoard (){
    int row, col;
    for (row = 0; row < BOARD_SIZE; row++) {
        fprintf(discFile,"%d| ", 8 - row);
        for (col = 0; col < BOARD_SIZE; col++) {
            fprintf(discFile,"%c ", Board[row][col]);
        }
        fprintf(discFile,"|\n");
    }
    fprintf(discFile,"  ");
    for (col = 0; col < 17; col++) {
        fprintf(discFile,(col == 16 ? "-\n" : "-"));
    }
    fprintf(discFile,"   ");
    for (col = 0; col < BOARD_SIZE; col++) {
        fprintf(discFile,(col == 7 ? "H\n" : "%c "), 'A' + col);
    }
}


/**------------ The Load Functions------------
 *
 */

/**
 *  param: char* Path -> contain the absolute or relative path of the file name to load from
 *  return: int -> 0 for error or 1 for success loading to file
 *  action: loading the format data to the correct variables from the file in a Path variable
 */


int LoadPrevGame(char * Path) {

    discFile = fopen(Path, "r");
    if(discFile == NULL) {
        return 0;
    }
    LoadSettings();
    LoadBoard();
    InitMovesHistory();
    if(fclose(discFile)!= 0){
        InitBoard();
        SetDefault();
        printf(ERROR , "fclose failed\n");
        return 0;
    };
    return 1;
}


/**
 *  param: none
 *  return: void
 *  action: loading the data format Settings using the fgets function from a file to the LoadBuffer
 */


void LoadSettings(){

    fgets(LoadBuffer, 30, discFile);
    LoadBuffer[strlen(LoadBuffer)-1] = '\0';
    // get the current color player
    if(strcmp(LoadBuffer , BLACK) == 0) CurrPlayerColor = BLACK;
    else CurrPlayerColor = WHITE;

    fgets(LoadBuffer , 30 , discFile);
    fgets(LoadBuffer , 30 , discFile);
    // set the game mode Global variable
    GameMode = LoadBuffer[11] - '0';
    if(GameMode == 1){
        fgets(LoadBuffer, 30, discFile);
        LoadBuffer[strlen(LoadBuffer) - 1] = '\0';
        SetDiff(LoadBuffer + 12); // set difficulty
        fgets(LoadBuffer, 30, discFile);
        LoadBuffer[strlen(LoadBuffer) - 1] = '\0';
        SetColorAfterLoading(LoadBuffer + 12); // set UserColor
    }
}


/**
 *  param: String diff of the saved difficulty
 *  return: void
 *  action: set the global difficulty variable to be 1-5
 */

void SetDiff(char * diff){
    if(strcmp(diff , "amateur") == 0) Difficulty = 1 ;
    else if(strcmp(diff , "easy") == 0) Difficulty = 2 ;
    else if(strcmp(diff , "moderate") == 0) Difficulty = 3 ;
    else if(strcmp(diff , "hard") == 0)  Difficulty = 4 ;
    else if(strcmp(diff , "expert") == 0) Difficulty = 5 ;
}


/**
 *  param: String color for the User Color
 *  return: void
 *  action: Update the user color and computer color
 */

void SetColorAfterLoading(char * color){
    if (strcmp(color , WHITE) == 0) {
        UserColor = WHITE;
        CompColor = BLACK;
    }
    else{
        UserColor = BLACK;
        CompColor = WHITE;
    }
}


/**
 *  param: none
 *  return: void
 *  action: Loading the data format Board using the fgets function from the file to the LoadBuffer
 *  and update the Board global variable
 */

void LoadBoard(){
    int i,j,k;
    for (i = 0 ; i < BOARD_SIZE ; i++){
        fgets(LoadBuffer, 30, discFile);
        k = 3;
        for (j = 0 ;j < BOARD_SIZE ; j++){
            char c;
            while( k < strlen(LoadBuffer)-1){
                c = LoadBuffer[k];
                k++;
                if(c != ' '){
                    Board[i][j] = c;
                    break;
                }
            }
        }
    }
}