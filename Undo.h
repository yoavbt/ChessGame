#ifndef FINAL_PROJECT_UNDO_H
#define FINAL_PROJECT_UNDO_H

#include "Gen.h"

#define EMPTY_HISTORY "Empty history, no move to undo\n"

extern char * CurrPlayerColor;
extern char Board[BOARD_SIZE][BOARD_SIZE];


//Initializations and updates: -------------------------------------------------------------------------------------

void InitMovesHistory();

void PutXBoard(char CurrBoard[BOARD_SIZE][BOARD_SIZE]);

void UpdateMovesHistory(char newMove[BOARD_SIZE][BOARD_SIZE], cell *from, cell *to);


//The Undo function and auxiliary functions: ------------------------------------------------------------------------

void UndoLastMove();

void popLastMove(char *WhoPlayedLast);

void printUndo(char *WhoPlayedLast);

void FreeMoveHistory();

#endif //FINAL_PROJECT_UNDO_H
