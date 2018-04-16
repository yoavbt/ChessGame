#ifndef FINAL_PROJECT_MINIMAX_H
#define FINAL_PROJECT_MINIMAX_H

#include "Gen.h"

extern int Difficulty;
extern char *CompColor;

/*A PieceMoves with one-move-list. It is a move -
 * The cell is the source, and the move.cell is the destination.
 */
PieceMoves *BestMove;


/*The struct contains a PieceMoves field (a cell and available moves from that cell)
 * and a next-field.
 */
typedef struct PieceMovesNode {
    PieceMoves *PieceMove;
    struct PieceMovesNode *Next;
} PieceMovesNode;


/*The struct is a list of PieceMovesNode.
 * It has one field that points to the beginning of the list.
 */
typedef struct PieceMovesList {
    PieceMovesNode *First;
} PieceMovesList;



//Initializations: -------------------------------------------------------------------------------------------------

PieceMovesNode *InitNode(PieceMoves *PM);

PieceMovesList *InitList();


//The main functions: -----------------------------------------------------------------------------------------------

PieceMoves *GetAIMove(char Board[BOARD_SIZE][BOARD_SIZE], char *Color);

int AlphaBeta(char Board[BOARD_SIZE][BOARD_SIZE], int depth, int alpha, int beta, char *CurrColor);


//Auxiliary functions: -------------------------------------------------------------------------------------------------


//On moves, pieceMoves etc.: ---------------------------------------------


void AddNodesToPieceMovesList(PieceMovesList *List, PieceMovesNode *Node);


//On game-boards : ---------------------------------------------

int IsBetterPriority(cell *From, cell *To);

PieceMovesList *BoardMoves(char Board[BOARD_SIZE][BOARD_SIZE], char *Color);

int ScoreBoardCal(char Board[BOARD_SIZE][BOARD_SIZE]);


//Memory release: ---------------------------------------------------------------------------------------------------

void FreePieceMovesNode(PieceMovesNode *PMNode);

void FreePieceMovesList(PieceMovesList *PMList);



#endif //FINAL_PROJECT_MINIMAX_H
