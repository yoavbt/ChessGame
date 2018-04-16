#ifndef FINAL_PROJECT_SAVELOADMODEL_H
#define FINAL_PROJECT_SAVELOADMODEL_H

#include "Undo.h"


extern char Board[BOARD_SIZE][BOARD_SIZE];
extern int GameMode; // 1 player vs comp mode or 2 for 2 player's
extern char *CurrPlayerColor; // color of player who plays first
// Only for PLAYER_VS_AI mode:
extern int Difficulty; // beatween 1-5
extern char *UserColor; // BLACK or WHITE
extern char *CompColor; // BLACK or WHITE



int SaveCurrGame(char *Path);
void SaveSettings();
void SaveBoard();
int LoadPrevGame(char *Path);
void LoadSettings();
void SetDiff(char *diff);
void SetColorAfterLoading(char *color);
void LoadBoard();

#endif //FINAL_PROJECT_SAVELOADMODEL_H
