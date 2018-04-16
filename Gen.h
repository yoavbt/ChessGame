#ifndef FINAL_PROJECT_GEN_H
#define FINAL_PROJECT_GEN_H

#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdio.h>


#define BOARD_SIZE 8
#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))
#define WHITE "white"
#define BLACK "black"
#define ERROR "ERROR:%s"
#define WHITE_PAWN 'm'
#define WHITE_BISHOP 'b'
#define WHITE_KNIGHT 'n'
#define WHITE_ROOK 'r'
#define WHITE_QUEEN 'q'
#define WHITE_KING 'k'
#define BLACK_PAWN 'M'
#define BLACK_BISHOP 'B'
#define BLACK_KNIGHT 'N'
#define BLACK_ROOK 'R'
#define BLACK_QUEEN 'Q'
#define BLACK_KING 'K'
#define EMPTY '_'
#define EMPTY_STRING ""
#define MAX_CMD 2000
#define NOT_VALID 'v'
#define KING "king"
#define QUEEN "queen"
#define ROOK "rook"
#define KNIGHT "knight"
#define BISHOP "bishop"
#define PAWN "pawn"


/*
 * Structures Definition
 */
typedef struct cell {
    int xUser; // row 1-8
    char yUser; // col A-H
    int xBoard; // 0-7
    int yBoard;//0-7
} cell;

typedef struct move {
    cell *Cell;
    struct move *next;
} move;

typedef struct PieceMoves {
    cell *Cell;
    move *listOfMoves;
} PieceMoves;


cell *InitCell(char *String);

cell *InitCellByBoardIndex(int x, int y);

void UpdateCellIndex(cell *Cell, int x, int y);

int IsSameCell(cell *Cell1, cell *Cell2);

void PrintCell(cell *cellToPrint , int , int);

move *InitMove(cell *Cell);

void FreeMove(move *MoveToDelete);

void FreeListOfMoves(move *ListOfMoves);

void PrintMove(move *MoveToPrint , int , int);


PieceMoves *InitPieceMoves(cell *cell);

void AddMovesToPieceMoves(PieceMoves *Piece, move *MoveToAdd);

int PrintPieceMoves(PieceMoves *PieceToPrint);

void FreePieceMoves(PieceMoves *PieceToFree);


// Auxilary functions
void GetInput(void);

void SetDefault();


void InitBoard(void);

void PrintBoard(void);

// BOARD FUNCTION
int IsEmpty(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell);

char GetPiece(char Board[BOARD_SIZE][BOARD_SIZE], cell *Curr);

int IsCellContainPiece(char Board[BOARD_SIZE][BOARD_SIZE], cell *CurrCell);

char *RivalColor(char *CurrPlayerColor);

char *GetType(char c);

char *GetPieceColor(char Piece);

void CopyBoard(char target[BOARD_SIZE][BOARD_SIZE], char source[BOARD_SIZE][BOARD_SIZE]);

void UpdateBoard(char Board[BOARD_SIZE][BOARD_SIZE], cell *from, cell *to);

int IsValidPos(cell *curr);

int IsTwoCellInSameColor(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell1, cell *Cell2);

char *GetColorOfCell(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell);

int IsMoveForward(cell *To, cell *From, char *Color);

//Move Function
int IsMoveLegal(char Board[BOARD_SIZE][BOARD_SIZE], cell *From, cell *To);

int IsMoveCapturePiece(move *temp);

cell *GetNextCell(cell *From, cell *To);

cell *GetDiagonalNeighbors(cell *Cell);

cell *GetStraitNeighbors(cell *Cell);

cell *GetKnightNeighbors(cell *Cell);

move *GetAllCells(char Board[BOARD_SIZE][BOARD_SIZE], cell *CurrPos, cell *Nieghbor);

int IsMoveThreatened(move *Move, cell *Cell);

PieceMoves *GetAllMoves(char Board[BOARD_SIZE][BOARD_SIZE], char *Color);

int NoMoreMoves(char Board[BOARD_SIZE][BOARD_SIZE], char *Player);

PieceMoves *GetPieceMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell);

PieceMoves *PawnMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell);

PieceMoves *KingMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell);

PieceMoves *KnightMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell);

PieceMoves *RookMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell);

PieceMoves *BishopMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell);

PieceMoves *QueenMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell);

int IsKingThreatend(char Board[BOARD_SIZE][BOARD_SIZE], char *Color);

int KingAux(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell, PieceMoves *result, int x, int y);

int AuxMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell, PieceMoves *result, int opcode, int ForNum);

void FindKingIndex(char Board[BOARD_SIZE][BOARD_SIZE], int *i, int *j, char *Color);

int IsStartedPawnLocation(cell *Cell, char *Color);

int IsValidPawnDiagMove(char Board[BOARD_SIZE][BOARD_SIZE], cell *Neighbor, cell *Curr, char *Color);

int IsValidPawnStrMove(char Board[BOARD_SIZE][BOARD_SIZE], cell *Neighbor, cell *Curr, char *Color);

#endif
