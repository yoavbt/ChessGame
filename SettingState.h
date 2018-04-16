#ifndef FINAL_PROJECT_SETTINGSTATE_H
#define FINAL_PROJECT_SETTINGSTATE_H

#include "SaveLoadModel.h"
#include "Undo.h"
// Massages
#define START_SETTING "Specify game settings or type 'start' to begin a game with the current settings:\n"
#define GAME_MODE_ERROR "Wrong game mode\n"
#define INVALID_CMD "ERROR: invalid command\n"
#define WRONG_DIFF_CMD "Wrong difficulty level. The value should be between 1 to 5\n"
#define WRONG_COLOR_CMD "Wrong user color. The value should be 0 or 1\n"
#define DEFAULT_SETTING_CMD "All settings reset to default\n"
#define EXIT_CMD "Exiting...\n"
#define START_CMD "Starting game...\n"
#define LOAD_ERR "Error: File doesn't exist or cannot be opened\n"


extern char Input[MAX_CMD];
extern int GameMode;
extern char *CurrPlayerColor;
extern int Difficulty;
extern char *UserColor;
extern char *CompColor;


int SettingState();

int ParsingInput();

void SetDifficulty(char*);

void SetColor(char c);

void PrintSettings(int GameMode);

void SetGameMode(char c);

#endif //FINAL_PROJECT_SETTINGSTATE_H
