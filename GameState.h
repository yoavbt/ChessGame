#ifndef FINAL_PROJECT_GAMESTATE_H
#define FINAL_PROJECT_GAMESTATE_H


#include "SaveLoadModel.h"
#include "Undo.h"
#include "MiniMax.h"

#define EXIT_CMD "Exiting...\n"
#define INVALID_CMD "ERROR: invalid command\n"
#define RESET_CMD "Restarting...\n"
#define INV_POS "Invalid position on the board\n"
#define NOT_CON_YOUR_PIECE "The specified position does not contain your piece\n"
#define NOT_CON_PLAYER_PIECE "The specified position does not contain a player piece\n"
#define ILL_MOV "Illegal move\n"
#define KING_STILL_THREATENED "Illegal move: king is still threatened\n"
#define KING_WILL_BE_THREATENED "Ilegal move: king will be threatened\n"
#define MATE "Checkmate! %s player wins the game\n"
#define CHECK "Check: %s king is threatened\n"
#define DRAW "The game ends in a draw\n"
#define SAVE_ERR "File cannot be created or modified\n"
#define SAVE_SUCC "Game saved to: %s\n"
#define AI_TURN "Computer: move %s at <%d,%c> to <%d,%c>\n"


extern char Board[BOARD_SIZE][BOARD_SIZE];
extern char Input[MAX_CMD];
extern int GameMode;
extern char* CurrPlayerColor;
extern int Difficulty;
extern char *UserColor;
extern char *CompColor;


int ComputerTurn();
int GameState(void);
int GameStatus(char *CurrColor);
int SetMove(char *Arg1, char *Arg3);
int GetMove(char*);
int CheckMoveSyntx(char *Arg);
int CheckMoveCmd(char *);
int CheckToCmd(char *To);
int ParsingUserTurn(void);
int UserTurn();

#endif //FINAL_PROJECT_GAMESTATE_H
