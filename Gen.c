#include "Gen.h"

// Global variables
// Board:
char Board[BOARD_SIZE][BOARD_SIZE];


// Game:
char Input[MAX_CMD];
int GameMode = 1; // 1 player vs comp mode or 2 for 2 player's
char *CurrPlayerColor= WHITE; // color of player who plays first


// Only for PLAYER_VS_AI mode:
int Difficulty = 2; // beatween 1-4
char *UserColor = WHITE; // BLACK or WHITE
char *CompColor = BLACK; // BLACK or WHITE


//Initializations, Auxiliary and Memory Release: ---------------------------------------------------------------------

//Cell functions: -----------------------------------------------------------
//Initializations: ---------------------------------


/*The function gets a string of the form <i,j>
 * and create a cell with the board index's i and j.
 * It returns a pointer to that cell, or NULL if there is memory error.
 */

cell *InitCell(char *String) {
    cell *result = (cell *) malloc(sizeof(cell));
    if (result == NULL) {
        return NULL;
    }
    result->xUser = String[1] - '0';
    result->yUser = String[3];
    result->xBoard = 8 - result->xUser;
    result->yBoard = result->yUser - 65;
    return result;
}

/*The function gets two integers x, y
 * and create a cell with the board index's x and y.
 * For valid cell, x and y supposed to be between 0 to 7.
 * It returns the cell itself, not the pointer to the cell.
 */

cell InitCellNotPointer(int x, int y) {
    cell Result;
    Result.xBoard = x;
    Result.yBoard = y;
    Result.xUser = 8 - x;
    Result.yUser = (char) ('A' + y);
    return Result;
}

/*The function gets two integers x, y
 * and create a cell with the board index's x and y.
 * For valid cell, x and y supposed to be between 0 to 7.
 * It returns the cell itself, not the pointer to the cell.
 */
cell *InitCellByBoardIndex(int x, int y) {

    cell *result = (cell *) malloc(sizeof(cell));
    if (result == NULL) {
        return NULL;
    }
    result->xUser = 8 - x;
    result->yUser = (char) ('A' + y);
    result->xBoard = x;
    result->yBoard = y;
    return result;
}

//Auxiliary functions: -------------------------------


/*The function gets a pointer to a cell, and return 1 if
 * its indexes between 0 to 7. Otherwise it returns 0.
 */
int IsValidPos(cell *curr) {
    return (curr->xBoard >= 0 && curr->yBoard >= 0 && curr->xBoard <= 7 && curr->yBoard <= 7);
}


/*The function gets a pointer to a cell and two integers x and y.
 * It updates the cell indexes to be x and y.
 * For valid cell, x and y supposed to be between 0 to 7.
 */
void UpdateCellIndex(cell *Cell, int x, int y) {
    Cell->xUser = 8 - x;
    Cell->yUser = (char) ('A' + y);
    Cell->xBoard = x;
    Cell->yBoard = y;
}

/*The function gets a pointer to a cell, and two flags (0 or 1).
 * It prints the cell in the format: <x,y> where x and y is
 * the cell indexes, at the user format.
 * If the captureFlag is 1, it adds ^ to the printing.
 * If the threatenedFlag is 1, it adds * to the printing.
 */

void PrintCell(cell *cellToPrint, int captureFlag, int threatenedFlag) {
    if (cellToPrint == NULL)printf("Not valid cell");
    if (captureFlag) {
        if (threatenedFlag) {
            printf("<%d,%c>*^", cellToPrint->xUser, cellToPrint->yUser);
        } else printf("<%d,%c>^", cellToPrint->xUser, cellToPrint->yUser);
    } else if (threatenedFlag) printf("<%d,%c>*", cellToPrint->xUser, cellToPrint->yUser);
    else printf("<%d,%c>", cellToPrint->xUser, cellToPrint->yUser);
}


/*The function gets two pointers to two cells.
 * It returns 1 if the indexes of the cells are equals.
 * Otherwise, it returns 0.
 */

int IsSameCell(cell *Cell1, cell *Cell2) {
    if (Cell1->xBoard == Cell2->xBoard && Cell1->yBoard == Cell2->yBoard)return 1;
    return 0;
}


/*The function gets two pointers to two cells.
 * It creates a new cell with indexes of the next cell in the
 * directions of the movement from the from-cell to the to-cell.
 * It returns a pointer to that cell.
 * If there is memory problem, it returns NULL.
 */

cell *GetNextCell(cell *From, cell *To) {
    return InitCellByBoardIndex(2 * To->xBoard - From->xBoard, 2 * To->yBoard - From->yBoard);
}



//Move functions: -----------------------------------------------------------

//Initializations: ---------------------------------


/*The function gets a pointer to a cell and creates a move
 * structure with that cell in his cell-field.
 * It returns a pointer to that move or NULL if there was a memory problem.
 */
move *InitMove(cell *Cell) {
    move *result = (move *) malloc(sizeof(move));
    if (result == NULL) return NULL;
    result->Cell = Cell;
    result->next = NULL;
    return result;
}

/*The function gets two integers and creates a move
 * structure with a cell with those indexes in his cell-field.
 * It returns a pointer to that move or NULL if there was a memory problem.
 */

move *InitMoveByBoardIndex(int x, int y) {
    cell *Cell = InitCellByBoardIndex(x, y);
    if (Cell == NULL)return NULL;
    move *result = InitMove(Cell);
    if (result == NULL)free(Cell);
    return result;
}

/*The function creates a move with NULL fields.
 * It returns a pointer to that move or NULL if there was a memory problem.
 */

move *InitEmptyMove() {
    move *result = (move *) malloc(sizeof(move));
    if (result == NULL) return NULL;
    result->Cell = NULL;
    result->next = NULL;
    return result;
}

//Auxiliary functions: ---------------------------------


/*The function gets a pointer to a move and two flags.
 * It prints the move.cell field with the given flags,
 * using the PrintCell function.
 */

void PrintMove(move *MoveToPrint, int captureFlag, int threatenedFlag) {
    PrintCell(MoveToPrint->Cell, captureFlag, threatenedFlag);
    printf("\n");
}


/*The function gets a pointer to a move, and
 * returns 0 if there is a piece colored the rival color in the move.cell
 * indexes on the game board.
 */

int IsMoveCapturePiece(move *temp) {
    if (temp == NULL) return 0;
    if (IsCellContainPiece(Board, temp->Cell)) {
        if (strcmp(GetPieceColor(GetPiece(Board, temp->Cell)), RivalColor(CurrPlayerColor)) == 0)return 1;
    }
    return 0;
};

/*The function gets a pointer to a move and a pointer to a cell.
 * It returns 1 if moving the piece from the cell to the move.cell
 * will make that piece threatened, and 0 otherwise.
 * If there is any memory problem, it returns 2.
 */

int IsMoveThreatened(move *Move, cell *Cell) {
    move *AllMoves = NULL;
    PieceMoves *pieceAllmoves = NULL;
    char Copy[BOARD_SIZE][BOARD_SIZE];
    char *Rival;
    move *temp;
    char *Color;
    CopyBoard(Copy, Board);
    Color = GetPieceColor(GetPiece(Copy, Cell));
    UpdateBoard(Copy, Cell, Move->Cell);
    Rival = strcmp(Color, BLACK) == 0 ? WHITE : BLACK;
    pieceAllmoves = GetAllMoves(Copy, Rival);
    if (pieceAllmoves == NULL) {
        return 2;
    }
    AllMoves = pieceAllmoves->listOfMoves;
    temp = AllMoves;
    while (temp != NULL) {
        if (IsSameCell(temp->Cell, Move->Cell)) {
            FreePieceMoves(pieceAllmoves);
            return 1;
        }
        temp = temp->next;
    }
    FreePieceMoves(pieceAllmoves);
    return 0;
}
//Memory Release: ---------------------------------------


/*The function gets a pointer to a move structure that
 *  has been allocated before and free it and so all his fields.
 */

void FreeMove(move *MoveToDelete) {
    if (MoveToDelete->Cell != NULL) {
        free(MoveToDelete->Cell);
    }
    MoveToDelete->Cell = NULL;
    MoveToDelete->next = NULL;
    free(MoveToDelete);
}


/*The function gets a pointer to a start of move list
 * and free all the moves in it.
 */

void FreeListOfMoves(move *ListOfMoves) {
    if (ListOfMoves != NULL) {
        FreeListOfMoves(ListOfMoves->next);
        ListOfMoves->next = NULL;
        FreeMove(ListOfMoves);
    }
}


//PieceMoves functions: ------------------------------------------------------

//Initialization: ---------------------------------------


/*The function gets a pointer to a cell and
 * create a PieceMoves structure with that cell in the cell-field.
 * It returns a pointer to that PieceMoves.
 * If there is memory error, it returns NULL.
 */


PieceMoves *InitPieceMoves(cell *cell) {
    PieceMoves *result = (PieceMoves *) malloc(sizeof(PieceMoves));
    if (result == NULL)return NULL;
    result->Cell = cell;
    result->listOfMoves = NULL;
    return result;
}

//Auxiliary functions: -------------------------------------


/*The function gets a pointer to a PieceMoves and a pointer to a move.
 * It adds the move to the beginning of the pieceMoves move-list field.
 */

void AddMovesToPieceMoves(PieceMoves *Piece, move *MoveToAdd) {
    move *last = MoveToAdd;
    if (Piece != NULL && MoveToAdd != NULL) {
        while (last->next != NULL)last = last->next;
        last->next = Piece->listOfMoves;
        Piece->listOfMoves = MoveToAdd;
    }
    if (Piece == NULL && MoveToAdd != NULL)
        Piece->listOfMoves = MoveToAdd;
}

/*The function gets a pointer to a PieceMoves,
 * and prints all the moves in it move-list field.
 * At every move, it checked if that move captures a piece,
 * or that move make the piece threatened, and print it with the
 * appropriate flags, using the PrintMove function.
 */


int PrintPieceMoves(PieceMoves *PieceToPrint) {
    move *temp = PieceToPrint->listOfMoves;
    int moveCapture;
    int moveThreatened;
    while (temp != NULL) {
        int LegalMove = IsMoveLegal(Board, PieceToPrint->Cell, temp->Cell);
        if (LegalMove == 1) {
            moveThreatened = IsMoveThreatened(temp, PieceToPrint->Cell);
            if (moveThreatened == 2) return 2;
            moveCapture = IsMoveCapturePiece(temp);
            if (moveCapture) {
                if (moveThreatened) {
                    PrintMove(temp, 1, 1);
                } else PrintMove(temp, 1, 0);
            } else if (moveThreatened) PrintMove(temp, 0, 1);
            else PrintMove(temp, 0, 0);
        } else if (LegalMove == 2)
            return 2;
        temp = temp->next;
    }
    return 1;
}


//Memory Release: -------------------------------------


/*The function gets a pointer to a PieceMoves structure that
 *  has been allocated before and free it, and so all his fields.
 */

//
//void FreePieceMoves(PieceMoves *PieceToFree) {
//    move *temp;
//    while (PieceToFree->listOfMoves != NULL) {
//        temp = PieceToFree->listOfMoves;
//        PieceToFree->listOfMoves = PieceToFree->listOfMoves->next;
//        FreeMove(temp);
//    }
//    free(PieceToFree->Cell);
//    PieceToFree->Cell = NULL;
//    PieceToFree->listOfMoves = NULL;
//    free(PieceToFree);
//}
void FreePieceMoves(PieceMoves *PieceToFree) {
    move *temp;
    if (PieceToFree == NULL)return;
    if (PieceToFree->listOfMoves != NULL) {
        while (PieceToFree->listOfMoves->next != NULL) {
            temp = PieceToFree->listOfMoves;
            PieceToFree->listOfMoves = PieceToFree->listOfMoves->next;
            FreeMove(temp);
        }
        FreeMove(PieceToFree->listOfMoves);
    }
    if (PieceToFree->Cell != NULL)
        free(PieceToFree->Cell);
    PieceToFree->Cell = NULL;
    PieceToFree->listOfMoves = NULL;
    free(PieceToFree);
}


//functions on the game board: --------------------------------------------------------------------------------

//Initializations: -------------------------------------


/*The function initialization the game board to be in size
 * BOARD_SIZE X BOARD_SIZE, and puts the appropriate pieces.
 */


void InitBoard() {
    int row, col;
    for (row = 0; row < BOARD_SIZE; row++) {
        for (col = 0; col < BOARD_SIZE; col++) {
            if (row == 1 || row == 6) {
                if (row == 1) {
                    Board[row][col] = BLACK_PAWN;
                } else Board[row][col] = WHITE_PAWN;
            } else if (row == 0 || row == 7) {
                if (col == 0 || col == 7) {
                    if (row == 0) Board[row][col] = BLACK_ROOK;
                    else Board[row][col] = WHITE_ROOK;
                }
                if (col == 1 || col == 6) {
                    if (row == 0) Board[row][col] = BLACK_KNIGHT;
                    else Board[row][col] = WHITE_KNIGHT;
                }
                if (col == 2 || col == 5) {
                    if (row == 0) Board[row][col] = BLACK_BISHOP;
                    else Board[row][col] = WHITE_BISHOP;
                }
                if (col == 3) {
                    if (row == 0) Board[row][col] = BLACK_QUEEN;
                    else Board[row][col] = WHITE_QUEEN;
                }
                if (col == 4) {
                    if (row == 0) Board[row][col] = BLACK_KING;
                    else Board[row][col] = WHITE_KING;
                }
            } else Board[row][col] = EMPTY;
        }
    }
}


//Auxiliary functions: -------------------------------------


/*The function prints the game-board with the
 * appropriate symbols - the axes, and separations symbols.
 */


void PrintBoard() {
    int row, col;
    for (row = 0; row < BOARD_SIZE; row++) {
        printf("%d| ", 8 - row);
        for (col = 0; col < BOARD_SIZE; col++) {
            printf("%c ", Board[row][col]);
        }
        printf("|\n");
    }
    printf("  ");
    for (col = 0; col < 17; col++) {
        printf((col == 16 ? "-\n" : "-"));
    }
    printf("   ");
    for (col = 0; col < BOARD_SIZE; col++) {
        printf((col == 7 ? "H\n" : "%c "), 'A' + col);
    }
}


/*The function gets a game board and a pointer to a cell,
 * and returns 1 if there is no piece in the cell's indexes on the
 * board. otherwise, it returns 0;
 */

int IsEmpty(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell) {
    if (Board[Cell->xBoard][Cell->yBoard] == EMPTY) return 1;
    return 0;
}


/*The function gets a game board and a pointer to a cell,
 * and returns a char - the piece in the cell's indexes on the
 * board. If the cell indexes are not valid, it returns the macro NOT_VALID.
 */

char GetPiece(char Board[BOARD_SIZE][BOARD_SIZE], cell *Curr) {
    if (IsValidPos(Curr)) {
        return Board[Curr->xBoard][Curr->yBoard];
    }
    return NOT_VALID;
}


/*The function gets a game board and a pointer to a cell,
 * and returns 1 if there is a piece in the cell's indexes on the
 * board. otherwise, it returns 0;
 */

int IsCellContainPiece(char Board[BOARD_SIZE][BOARD_SIZE], cell *CurrCell) {
    if (strcmp(GetType(Board[CurrCell->xBoard][CurrCell->yBoard]), EMPTY_STRING) == 0) return 0;
    return 1;
}


/*The function gets a char (supposed to be a char in the game-board)
 * and returns the piece-type of that char.
 */

char *GetType(char c) {
    if (c == WHITE_PAWN || c == BLACK_PAWN) {
        return PAWN;
    } else if (c == WHITE_BISHOP || c == BLACK_BISHOP) {
        return BISHOP;
    } else if (c == WHITE_KNIGHT || c == BLACK_KNIGHT) {
        return KNIGHT;
    } else if (c == WHITE_ROOK || c == BLACK_ROOK) {
        return ROOK;
    } else if (c == WHITE_QUEEN || c == BLACK_QUEEN) {
        return QUEEN;
    } else if (c == WHITE_KING || c == BLACK_KING) {
        return KING;
    } else
        return EMPTY_STRING;
}

/*The function gets a char (supposed to be a char in the game-board)
 * and returns the color of that char (black or white).
 * If the char is not from the game-board, it returns the macro EMPTY_STRING.
 */

char *GetPieceColor(char Piece) {
    if (Piece == WHITE_KING || Piece == WHITE_QUEEN || Piece == WHITE_BISHOP
        || Piece == WHITE_KNIGHT || Piece == WHITE_ROOK || Piece == WHITE_PAWN)
        return WHITE;
    else if (Piece == BLACK_KING || Piece == BLACK_QUEEN || Piece == BLACK_BISHOP
             || Piece == BLACK_KNIGHT || Piece == BLACK_ROOK || Piece == BLACK_PAWN)
        return BLACK;
    return EMPTY_STRING;
}

/*The function gets a color (black or white).
 * If its black, it returns white. Otherwise, it returns black.
 */

char *RivalColor(char *CurrPlayerColor) {
    if (strcmp(CurrPlayerColor, BLACK) == 0) return WHITE;
    return BLACK;
}



/*The functions gets two game-boards and copy the cells from the
 * source game board to the target game board.
 */

void CopyBoard(char target[BOARD_SIZE][BOARD_SIZE], char source[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            target[i][j] = source[i][j];
        }
    }
}

/*The function gets a game board and two pointers to valid cells.
 * It moves the piece in the from-cell to the indexes of the to-cell.
 */
void UpdateBoard(char Board[BOARD_SIZE][BOARD_SIZE], cell *from, cell *to) {
    Board[to->xBoard][to->yBoard] = GetPiece(Board, from);
    Board[from->xBoard][from->yBoard] = EMPTY;
}



/*The function gets a game board and pointers to two cells.
 * It returns 1 if the pieces in those cell's indexes has the same color.
 * Otherwise, it returns 0;
 */




int IsTwoCellInSameColor(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell1, cell *Cell2) {
    if (strcmp(GetColorOfCell(Board, Cell1), GetColorOfCell(Board, Cell2)) == 0)return 1;
    return 0;
}

/*The function gets a game board and a pointer to a cell.
 * and returns the color of the piece placed on the game board on the
 * cell's indexes (black or white).
 * If there is no piece on it, it returns the empty string.
 */

char *GetColorOfCell(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell) {
    char c = Board[Cell->xBoard][Cell->yBoard];
    if (c == EMPTY)return "";
    else if (c == WHITE_PAWN || c == WHITE_BISHOP || c == WHITE_ROOK || c == WHITE_KNIGHT
             || c == WHITE_KING || c == WHITE_QUEEN)
        return WHITE;
    return BLACK;
}


/*The function gets a pointers to two cells and a color (black or white).
 * It returns 1 if the movement from the piece located in the from-cell
 * to the to-cell is a forward move. Otherwise, it returns 0.
 */

int IsMoveForward(cell *To, cell *From, char *Color) {
    if (strcmp(Color, BLACK) == 0) {
        if (To->xBoard > From->xBoard) return 1;
    } else if (strcmp(Color, WHITE) == 0) {
        if (To->xBoard < From->xBoard) return 1;
    }
    return 0;
}

/*The function gets a game board and pointers to two cell.
 *If the movement from the piece located in the from-cell
 *to the to-cell makes the king with the same color threatened,
 *it returns 0. Otherwise, it returns 1.
 *If there is any memory error, it returns 2.
 */
int IsMoveLegal(char Board[BOARD_SIZE][BOARD_SIZE], cell *From, cell *To) {
    int Result;
    char *color = GetColorOfCell(Board, From);
    char Copy[BOARD_SIZE][BOARD_SIZE];
    CopyBoard(Copy, Board);
    UpdateBoard(Copy, From, To);
    Result = IsKingThreatend(Copy, color);
    if (Result == 1) return 0;
    else if (Result == 0) return 1;
    return 2;
}


//Get Neighbors functions: --------------------------------------------


/*The function gets a pointer to a cell
 * and creates 4 cells - all the diagonal neighbors cells
 * of that cell. Warning: the cells not necessarily valid.
 * If there is memory error, it returns NULL.
 */

cell *GetDiagonalNeighbors(cell *Cell) {
    cell *N = malloc(sizeof(cell) * 4);
    if (N == NULL)
        return NULL;
    N[0] = InitCellNotPointer(Cell->xBoard + 1, Cell->yBoard + 1);
    N[1] = InitCellNotPointer(Cell->xBoard + 1, Cell->yBoard - 1);
    N[2] = InitCellNotPointer(Cell->xBoard - 1, Cell->yBoard - 1);
    N[3] = InitCellNotPointer(Cell->xBoard - 1, Cell->yBoard + 1);
    return N;
}


/*The function gets a pointer to a cell
 * and creates 4 cells - all the strait neighbors cells
 * of that cell. Warning: the cells not necessarily valid.
 * If there is memory error, it returns NULL.
 */


cell *GetStraitNeighbors(cell *Cell) {
    cell *N = malloc(sizeof(cell) * 4);
    if (N == NULL)
        return NULL;
    N[0] = InitCellNotPointer(Cell->xBoard, Cell->yBoard + 1);
    N[1] = InitCellNotPointer(Cell->xBoard, Cell->yBoard - 1);
    N[2] = InitCellNotPointer(Cell->xBoard - 1, Cell->yBoard);
    N[3] = InitCellNotPointer(Cell->xBoard + 1, Cell->yBoard);
    return N;
}


/*The function gets a pointer to a cell (supposed to be a knight)
 * and creates 8 cells - all the valid moves from that cell
 * for a knight. Warning: the cells not necessarily valid.
 * If there is memory error, it returns NULL.
 */

cell *GetKnightNeighbors(cell *Cell) {
    cell *N = malloc(sizeof(cell) * 8);
    if (N == NULL)
        return NULL;
    N[0] = InitCellNotPointer(Cell->xBoard + 2, Cell->yBoard + 1);
    N[1] = InitCellNotPointer(Cell->xBoard + 2, Cell->yBoard - 1);
    N[2] = InitCellNotPointer(Cell->xBoard + 1, Cell->yBoard - 2);
    N[3] = InitCellNotPointer(Cell->xBoard + 1, Cell->yBoard + 2);
    N[4] = InitCellNotPointer(Cell->xBoard - 1, Cell->yBoard + 2);
    N[5] = InitCellNotPointer(Cell->xBoard - 1, Cell->yBoard - 2);
    N[6] = InitCellNotPointer(Cell->xBoard - 2, Cell->yBoard + 1);
    N[7] = InitCellNotPointer(Cell->xBoard - 2, Cell->yBoard - 1);
    return N;
}


/*The function gets a game board and pointers for two cells.
 * It return a move - a beginning of valid moves list that
 * in same direction like the movement from the CurrPos cell
 * to his Nieghbor cell.
 * If there is any memory error, it returns NULL.
 */


move *GetAllCells(char Board[BOARD_SIZE][BOARD_SIZE], cell *CurrPos, cell *Nieghbor) {
    move *head = NULL;
    move *curr;
    move *next = NULL;
    cell *CopyOfNieghbor = NULL;
    cell *From;
    cell *To;
    cell *Temp = NULL;
    if (IsValidPos(Nieghbor)) {
        if (IsTwoCellInSameColor(Board, CurrPos, Nieghbor)) {
            return InitEmptyMove();
        } else {
            CopyOfNieghbor = InitCellByBoardIndex(Nieghbor->xBoard, Nieghbor->yBoard);
            if (CopyOfNieghbor == NULL) return NULL;
            curr = InitMove(CopyOfNieghbor);
            if (curr == NULL) {
                free(CopyOfNieghbor);
                return NULL;
            }
            head = curr;
            if (strcmp(GetColorOfCell(Board, Nieghbor), EMPTY_STRING) == 0) {
                To = GetNextCell(CurrPos, Nieghbor);
                if (To == NULL) {
                    FreeListOfMoves(head);
                    return NULL;
                }
                From = InitCellByBoardIndex(Nieghbor->xBoard, Nieghbor->yBoard);
                if (From == NULL) {
                    free(To);
                    FreeListOfMoves(head);
                    return NULL;
                }
                while (IsValidPos(To) && (strcmp(GetColorOfCell(Board, From), EMPTY_STRING) == 0)) {
                    cell *CopyOfTo = InitCellByBoardIndex(To->xBoard, To->yBoard);
                    if (CopyOfTo == NULL) {
                        free(To);
                        free(From);
                        FreeListOfMoves(head);
                        return NULL;
                    }
                    next = InitMove(CopyOfTo);
                    if (next == NULL) {
                        free(CopyOfTo);
                        free(To);
                        free(From);
                        FreeListOfMoves(head);
                        return NULL;
                    }
                    curr->next = next;
                    curr = curr->next;
                    Temp = To;
                    To = GetNextCell(From, To);
                    free(From);
                    if (To == NULL) {
                        free(Temp);
                        FreeListOfMoves(head);
                        return NULL;
                    }
                    From = Temp;
                }
                if (IsValidPos(To)) {
                    if (!IsTwoCellInSameColor(Board, To, CurrPos)) {
                        cell *CopyOfTo = InitCellByBoardIndex(To->xBoard, To->yBoard);
                        if (CopyOfTo == NULL) {
                            free(To);
                            free(From);
                            FreeListOfMoves(head);
                            return NULL;
                        }
                        next = InitMove(CopyOfTo);
                        if (next == NULL) {
                            free(CopyOfTo);
                            free(To);
                            free(From);
                            FreeListOfMoves(head);
                            return NULL;
                        }
                        curr->next = next;
                    }
                }
                if (Temp == NULL) free(From);
                else free(Temp);
                free(To);
            }
            return head;
        }
    } else return InitEmptyMove();

}

//General functions: ---------------------------------------------------------------------------------------


/*The function gets an input to
 * the Input array.
 */
// GENERAL FUNCTION
void GetInput() {
    fgets(Input, MAX_CMD, stdin);
    int i;
    i = 0;
    while (Input[i] != '\n') {
        i++;
    }
    Input[i] = '\0';
}

void SetDefault() {
    CurrPlayerColor = WHITE;
    GameMode = 1;
    UserColor = WHITE;
    CompColor = BLACK;
    Difficulty = 2;
}


// returns all moves for a <color> player or NULL for memory issues;
PieceMoves *GetAllMoves(char Board[BOARD_SIZE][BOARD_SIZE], char *Color) {
    PieceMoves *curr = NULL;
    PieceMoves *AllPiece = NULL;
    PieceMoves *res;
    cell * Cell = InitCellByBoardIndex(0,0);
    if(Cell == NULL){
        return NULL;
    }
    AllPiece = InitPieceMoves(Cell);
    if(AllPiece == NULL){
        free(Cell);
        return NULL;
    }
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (strcmp(GetPieceColor(Board[i][j]), Color) == 0) {
                cell *tempCell = InitCellByBoardIndex(i, j);
                if(tempCell == NULL){
                    FreePieceMoves(AllPiece);
                    return NULL;
                }
                curr = GetPieceMoves(Board, tempCell);
                if(curr == NULL){
                    FreePieceMoves(AllPiece);
                    return NULL;
                }
                if (curr->listOfMoves == NULL) {
                    FreePieceMoves(curr);
                }
                else {
                    AddMovesToPieceMoves(AllPiece, curr->listOfMoves);
                }
            }
        }
    }
    res = AllPiece;
    return res;
}


/*The function gets a game board and color (white or black)
 * and returns 1 if there is no board piece that the player with
 * that color can do. Otherwise it returns 0.
 * If there is any memory error, it returns 2.
 */

 int NoMoreMoves(char Board[BOARD_SIZE][BOARD_SIZE], char *Player) {
    PieceMoves *currPiece = NULL;
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            cell *Cell = InitCellByBoardIndex(i, j);
            if (Cell == NULL) return 2;
            else if (strcmp(GetColorOfCell(Board, Cell), Player) == 0) {
                currPiece = GetPieceMoves(Board, Cell);
                if (currPiece == NULL) {
                    free(Cell);
                    return 2;
                }
                if (currPiece->listOfMoves != NULL){
                    move* temp = currPiece->listOfMoves;
                    while (temp != NULL){
                        if (IsMoveLegal(Board, currPiece->Cell , temp->Cell)){
                            FreePieceMoves(currPiece);
                            return 0;
                        }
                        temp = temp->next;
                    }
                }
                FreePieceMoves(currPiece);
            }
            else free(Cell);
        }
    }
    return 1;
}



//Get Moves functions: ---------------------------------------------------------------------------------------


/*The function gets a game board and a cell.
 * It returns a PieceMoves structure that it
 * cell is copy of that cell, and it moves list
 * field is a list of all available moves that
 * the piece on this cell can do.
 * for an empty cell it return empty Piece moves that
 * has a property listOfMoves == NULL
 * If there is any memory error, it returns NULL.
 */

PieceMoves *GetPieceMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell) {
    char c = Board[Cell->xBoard][Cell->yBoard];
    switch (c) {
        case WHITE_PAWN:
            return PawnMoves(Board, Cell);
        case WHITE_ROOK:
            return RookMoves(Board, Cell);
        case WHITE_BISHOP:
            return BishopMoves(Board, Cell);
        case WHITE_KNIGHT:
            return KnightMoves(Board, Cell);
        case WHITE_QUEEN:
            return QueenMoves(Board, Cell);
        case WHITE_KING:
            return KingMoves(Board, Cell);
        case BLACK_PAWN:
            return PawnMoves(Board, Cell);
        case BLACK_ROOK:
            return RookMoves(Board, Cell);
        case BLACK_BISHOP:
            return BishopMoves(Board, Cell);
        case BLACK_KNIGHT:
            return KnightMoves(Board, Cell);
        case BLACK_QUEEN:
            return QueenMoves(Board, Cell);
        case BLACK_KING:
            return KingMoves(Board, Cell);
        default:
            break;
    }
    cell *cellForEmpty = InitCellByBoardIndex(0 , 0);
    if (cellForEmpty == NULL){
        return NULL;
    }
    PieceMoves * empty = InitPieceMoves(cellForEmpty);
    if(empty == NULL){
        free(cellForEmpty);
        return NULL;
    }
    return empty;
}


//Pawn functions: ---------------------------------

/*The function gets a game board and a pointer to a cell
 * (its supposed to be a pawn on that cell). It returns a PieceMoves structure that it
 * cell is copy of that cell, and it moves list
 * field is a list of all available pawn moves that
 * the pawn on this cell can do.
 * If there is any memory error, it returns NULL.
 */


PieceMoves *PawnMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell) {
    cell *DiagonalN = NULL;
    cell *StraightN = NULL;
    int i;
    char *Color = GetColorOfCell(Board, Cell);
    PieceMoves *result = InitPieceMoves(Cell);
    if (result == NULL) return NULL;
    DiagonalN = GetDiagonalNeighbors(Cell);
    if (DiagonalN != NULL) {
        for (i = 0; i < 4; i++) {
            cell tempCell = DiagonalN[i];
            if (IsValidPawnDiagMove(Board, &tempCell, Cell, Color)) {
                move *Move = InitMoveByBoardIndex(tempCell.xBoard, tempCell.yBoard);
                if (Move == NULL) {
                    free(DiagonalN);
                    FreePieceMoves(result);
                    return NULL;
                }
                AddMovesToPieceMoves(result, Move);
            }
        }
        free(DiagonalN);
    }
    StraightN = GetStraitNeighbors(Cell);
    if (StraightN != NULL) {
        for (i = 0; i < 4; i++) {
            cell tempCell = StraightN[i];
            if (IsValidPawnStrMove(Board, &tempCell, Cell, Color)) {
                move *Move = InitMoveByBoardIndex(tempCell.xBoard, tempCell.yBoard);
                if (Move == NULL) {
                    free(StraightN);
                    FreePieceMoves(result);
                    return NULL;
                }
                AddMovesToPieceMoves(result, Move);
            }
        }
        free(StraightN);
    }
    if (IsStartedPawnLocation(Cell, Color) == 1) {
        if (strcmp(Color, WHITE) == 0 && Board[Cell->xBoard - 1][Cell->yBoard] == EMPTY
            && Board[Cell->xBoard - 2][Cell->yBoard] == EMPTY) {
            move *Move = InitMoveByBoardIndex(Cell->xBoard - 2, Cell->yBoard);
            if (Move == NULL) {
                FreePieceMoves(result);
                return NULL;
            }
            AddMovesToPieceMoves(result, Move);
        }
        if (strcmp(Color, BLACK) == 0 && Board[Cell->xBoard + 1][Cell->yBoard] == EMPTY
            && Board[Cell->xBoard + 2][Cell->yBoard] == EMPTY) {
            move *Move = InitMoveByBoardIndex(Cell->xBoard + 2, Cell->yBoard);
            if (Move == NULL) {
                FreePieceMoves(result);
                return NULL;
            }
            AddMovesToPieceMoves(result, Move);
        }
    }
    return result;
}

/*The function gets a pointer to a valid cell (its supposed to be a pawn on that cell)
 * and a color (white or black), and returns 1 if the pawn
 * located on his beginning locations. Otherwise, it returns 0.
 */

int IsStartedPawnLocation(cell *Cell, char *Color) {
    if (strcmp(Color, WHITE) == 0 && Cell->xBoard == BOARD_SIZE - 2)
        return 1;
    if (strcmp(Color, BLACK) == 0 && Cell->xBoard == 1)
        return 1;
    return 0;
}

/*The function gets two pointer to cells and a color (white or black),
 * and returns 1 if the pawn located in the Curr cell can
 * move diagonaly to the Neighbor cell, i.e. there is an enemy on this cell.
 * Otherwise, it return 0.
 */

int IsValidPawnDiagMove(char Board[BOARD_SIZE][BOARD_SIZE], cell *Neighbor, cell *Curr, char *Color) {
    return (IsValidPos(Neighbor) && (strcmp(GetColorOfCell(Board, Neighbor), GetColorOfCell(Board, Curr)) != 0)
            && (strcmp(GetColorOfCell(Board, Neighbor), EMPTY_STRING) != 0) && IsMoveForward(Neighbor, Curr, Color));
}

/*The function gets two pointer to cells and a color (white or black),
 * and returns 1 if the pawn located in the Curr cell can
 * move straitly to the Neighbor cell, i.e. there is no one on this cell.
 * Otherwise, it return 0.
 */

int IsValidPawnStrMove(char Board[BOARD_SIZE][BOARD_SIZE], cell *Neighbor, cell *Curr, char *Color) {
    return IsValidPos(Neighbor) && IsMoveForward(Neighbor, Curr, Color) && IsEmpty(Board, Neighbor);
}

//King functions: ---------------------------------

/*The function gets a game board and a pointer to a cell
 * (its supposed to be a king on that cell). It returns a PieceMoves structure that it
 * cell is copy of that cell, and it moves list
 * field is a list of all available king moves that
 * the king on this cell can do.
 * If there is any memory error, it returns NULL.
 */

PieceMoves *KingMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell) {
    PieceMoves *result = InitPieceMoves(Cell);
    if (result == NULL) return NULL;
    if (KingAux(Board, Cell, result, 1, 1) == 0)
        return NULL;
    if (KingAux(Board, Cell, result, 1, 0) == 0)
        return NULL;
    if (KingAux(Board, Cell, result, 1, -1) == 0)
        return NULL;
    if (KingAux(Board, Cell, result, 0, -1) == 0)
        return NULL;
    if (KingAux(Board, Cell, result, -1, -1) == 0)
        return NULL;
    if (KingAux(Board, Cell, result, -1, 0) == 0)
        return NULL;
    if (KingAux(Board, Cell, result, -1, 1) == 0)
        return NULL;
    if (KingAux(Board, Cell, result, 0, 1) == 0)
        return NULL;
    return result;
}

/*The function is an auxiliary function for the KingMoves function.
 * It gets a game board, a cell (its supposed to be a king on that cell),
 * an exist PieceMoves and two integers x and y.
 * The function checks if the move from the Cell to a cell
 * with shift of x and y from the Cell is valid for the king located in Cell.
 * if so, it adds the appropriate move to the result moves list.
 * If there is any memory error, it returns NULL.
 */


int KingAux(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell, PieceMoves *result, int x, int y) {
    cell *Dest = InitCellByBoardIndex(Cell->xBoard + x, Cell->yBoard + y);
    if (Dest == NULL) return 0;
    //If there is no one there or an enemy
    if (IsValidPos(Dest) == 1 && (IsEmpty(Board, Dest) == 1 ||
                                  IsTwoCellInSameColor(Board, Dest, Cell) == 0)) {
        char CpyBoard[BOARD_SIZE][BOARD_SIZE];
        CopyBoard(CpyBoard, Board);
        UpdateBoard(CpyBoard, Cell, Dest);
        move *Move = InitMove(Dest);
            if (Move == NULL) {
                free(Dest);
                return 0;
        }
        AddMovesToPieceMoves(result, Move);
    }
    return 1;
}

/*The function gets a game board, pointers for two integers and color (white or black).
 * It updates the integers to indexes on the game board where the king
 * with the given color is.
 */

void FindKingIndex(char Board[BOARD_SIZE][BOARD_SIZE], int *i, int *j, char *Color) {
    int x;
    int y;
    for (x = 0; x < BOARD_SIZE; x++) {
        for (y = 0; y < BOARD_SIZE; y++) {
            if (strcmp(GetType(Board[x][y]), KING) == 0 && strcmp(GetPieceColor(Board[x][y]), Color) == 0) {
                *i = x;
                *j = y;
            }
        }
    }
}


/*The function gets a game board and a color (white or black).
 * It returns 1 if the king with that color is threatened, i.e
 * there is a piece that can captures this king.
 * Otherwise, it returns 0.
 * If there is any memory error, it returns 2.
 */

int IsKingThreatend(char Board[BOARD_SIZE][BOARD_SIZE], char *Color) {
    char Piece;
    int i, j;
    char *CurrPiece;
    cell *DiagN = NULL;
    cell *StrN = NULL;
    cell *KnightN = NULL;
    move *tempD = NULL;
    move *CurrMove = NULL;
    FindKingIndex(Board, &i, &j, Color);
    cell *KingCell = InitCellByBoardIndex(i, j);
    KnightN = GetKnightNeighbors(KingCell);
    if (KnightN == NULL) return 2;
    for (i = 0; i < 8; i++) {
        cell Curr = KnightN[i];
        if (IsValidPos(&Curr)) {
            Piece = Board[Curr.xBoard][Curr.yBoard];
            if ((Piece == BLACK_KNIGHT && strcmp(Color, WHITE) == 0)
                || (Piece == WHITE_KNIGHT && strcmp(Color, BLACK) == 0)) {
                free(KingCell);
                free(KnightN);
                return 1;
            }
        }
    }
    free(KnightN);
    DiagN = GetDiagonalNeighbors(KingCell);
    if (DiagN == NULL) return 2;
    for (i = 0; i < 4; i++) {
        tempD = GetAllCells(Board, KingCell, &DiagN[i]);
        CurrMove = tempD;
        if (tempD == NULL) {
            free(KingCell);
            free(DiagN);
            return 2;
        } else if (tempD->Cell == NULL) {
            FreeMove(tempD);
        } else {
            CurrPiece = GetType(Board[CurrMove->Cell->xBoard][CurrMove->Cell->yBoard]);
            if (strcmp(CurrPiece, PAWN) == 0 &&
                IsMoveForward(KingCell, CurrMove->Cell, GetColorOfCell(Board, CurrMove->Cell)) &&
                strcmp(Color, GetPieceColor(Board[CurrMove->Cell->xBoard][CurrMove->Cell->yBoard])) != 0) {
                free(KingCell);
                free(DiagN);
                return 1;
            } else if (strcmp(CurrPiece, KING) == 0 &&
                       strcmp(Color, GetPieceColor(Board[CurrMove->Cell->xBoard][CurrMove->Cell->yBoard])) != 0) {
                free(KingCell);
                free(DiagN);
                return 1;
            }
            while (CurrMove != NULL) {
                CurrPiece = GetType(Board[CurrMove->Cell->xBoard][CurrMove->Cell->yBoard]);
                if ((strcmp(CurrPiece, BISHOP) == 0 || strcmp(CurrPiece, QUEEN) == 0) &&
                    strcmp(Color, GetPieceColor(Board[CurrMove->Cell->xBoard][CurrMove->Cell->yBoard])) != 0) {
                    free(KingCell);
                    free(DiagN);
                    return 1;
                }
                if (strcmp(CurrPiece, "") != 0)
                    break;
                CurrMove = CurrMove->next;
            }
            FreeListOfMoves(tempD);
        }
    }
    free(DiagN);
    StrN = GetStraitNeighbors(KingCell);
    if (StrN == NULL) {
        free(KingCell);
        return 2;
    }
    for (i = 0; i < 4; i++) {
        tempD = GetAllCells(Board, KingCell, &StrN[i]);
        CurrMove = tempD;
        if (tempD == NULL) {
            free(KingCell);
            free(StrN);
            return 2;
        } else if (tempD->Cell == NULL) {
            FreeMove(tempD);
        } else {
            CurrPiece = GetType(Board[CurrMove->Cell->xBoard][CurrMove->Cell->yBoard]);
            if (strcmp(CurrPiece, KING) == 0 &&
                strcmp(Color, GetPieceColor(Board[CurrMove->Cell->xBoard][CurrMove->Cell->yBoard])) != 0) {
                free(KingCell);
                free(StrN);
                return 1;
            }
            while (CurrMove != NULL) {
                CurrPiece = GetType(Board[CurrMove->Cell->xBoard][CurrMove->Cell->yBoard]);
                if ((strcmp(CurrPiece, ROOK) == 0 || strcmp(CurrPiece, QUEEN) == 0) &&
                    strcmp(Color, GetPieceColor(Board[CurrMove->Cell->xBoard][CurrMove->Cell->yBoard])) != 0) {
                    free(KingCell);
                    free(DiagN);
                    return 1;
                }
                if (strcmp(CurrPiece, "") != 0)
                    break;
                CurrMove = CurrMove->next;
            }
            FreeListOfMoves(tempD);
        }
    }
    free(StrN);
    free(KingCell);
    return 0;
}

//Knight function: ---------------------------------

/*The function gets a game board and a pointer to a cell
 * (its supposed to be a Knight on that cell). It returns a PieceMoves structure that it
 * cell is copy of that cell, and it moves list
 * field is a list of all available Knight moves that
 * the Knight on this cell can do.
 * If there is any memory error, it returns NULL.
 */


PieceMoves *KnightMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell) {
    int i;
    PieceMoves *result = InitPieceMoves(Cell);
    if (result == NULL) return NULL;
    cell *knightNeighbors = GetKnightNeighbors(Cell);
    if (knightNeighbors == NULL) {
        FreePieceMoves(result);
        return NULL;
    }

    for (i = 0; i < 8; i++) {
        cell temp = knightNeighbors[i];
        if (IsValidPos(&temp) == 1 && (IsEmpty(Board, &temp) == 1 || !IsTwoCellInSameColor(Board, &temp, Cell))) {
            move *Move = InitMoveByBoardIndex(temp.xBoard, temp.yBoard);
            if (Move == NULL) {
                FreePieceMoves(result);
                free(knightNeighbors);
                return NULL;
            }
            AddMovesToPieceMoves(result, Move);
        }
    }
    free(knightNeighbors);
    return result;
}

//Rook function: ---------------------------------

/*The function gets a game board and a pointer to a cell
 * (its supposed to be a Rook on that cell). It returns a PieceMoves structure that it
 * cell is copy of that cell, and it moves list
 * field is a list of all available Rook moves that
 * the Rook on this cell can do.
 * If there is any memory error, it returns NULL.
 */

PieceMoves *RookMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell) {
    int AuxAns = 0;
    PieceMoves *result = InitPieceMoves(Cell);
    if (result == NULL) return NULL;
    //right move
    AuxAns = AuxMoves(Board, Cell, result, 5, BOARD_SIZE - Cell->yBoard - 1);
    if (AuxAns == 0) {
        FreePieceMoves(result);
        return NULL;
    }

    //left move
    AuxAns = AuxMoves(Board, Cell, result, 6, Cell->yBoard);
    if (AuxAns == 0) {
        FreePieceMoves(result);
        return NULL;
    }

    //up move
    AuxAns = AuxMoves(Board, Cell, result, 7, Cell->xBoard);
    if (AuxAns == 0) {
        FreePieceMoves(result);
        return NULL;
    }

    //down move
    AuxAns = AuxMoves(Board, Cell, result, 8, BOARD_SIZE - Cell->xBoard - 1);
    if (AuxAns == 0) {
        FreePieceMoves(result);
        return NULL;
    }

    return result;
}

//Bishop functions: ---------------------------------

/*The function gets a game board and a pointer to a cell
 * (its supposed to be a Bishop on that cell). It returns a PieceMoves structure that it
 * cell is copy of that cell, and it moves list
 * field is a list of all available Bishop moves that
 * the Bishop on this cell can do.
 * If there is any memory error, it returns NULL.
 */


PieceMoves *BishopMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell) {
    int AuxAns = 0;
    PieceMoves *result = InitPieceMoves(Cell);
    if (result == NULL) return NULL;

    //move right and up diagonally
    AuxAns = AuxMoves(Board, Cell, result, 1, MIN(BOARD_SIZE - Cell->yBoard - 1, Cell->xBoard));
    if (AuxAns == 0) {
        FreePieceMoves(result);
        return NULL;
    }

    //move left and up diagonally
    AuxAns = AuxMoves(Board, Cell, result, 2, MIN(Cell->yBoard, Cell->xBoard));
    if (AuxAns == 0) {
        FreePieceMoves(result);
        return NULL;
    }

    //move right and down diagonally
    AuxAns = AuxMoves(Board, Cell, result, 3, MIN(BOARD_SIZE - Cell->xBoard - 1, BOARD_SIZE - Cell->yBoard - 1));
    if (AuxAns == 0) {
        FreePieceMoves(result);
        return NULL;
    }

    //move left and down diagonally
    AuxAns = AuxMoves(Board, Cell, result, 4, MIN(Cell->yBoard, BOARD_SIZE - Cell->xBoard - 1));
    if (AuxAns == 0) {
        FreePieceMoves(result);
        return NULL;
    }

    return result;
}

/*The function is an auxiliary function for the BishopMoves and RookMoves functions.
 * It gets a game board, a cell, an exist PieceMoves and two
 *  integers - opcode(1-4 for bishop, 5-8 for rook. each one for other direction)
 *  and ForNum - how many tests to do in the opcode direction.
 * The function adds the appropriate move to the result moves list.
 * If there is any memory error, it free the local variables and return 0.
 */

int AuxMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell, PieceMoves *result, int opcode, int ForNum) {
    int i;
    cell *tmpCell = InitCellByBoardIndex(0, 0);
    if (tmpCell == NULL) return 0;
    for (i = 1; i <= ForNum; i++) {
        switch (opcode) {
            case 1://1 - 4 bishop
                UpdateCellIndex(tmpCell, Cell->xBoard - i, Cell->yBoard + i);
                break;
            case 2:
                UpdateCellIndex(tmpCell, Cell->xBoard - i, Cell->yBoard - i);
                break;
            case 3:
                UpdateCellIndex(tmpCell, Cell->xBoard + i, Cell->yBoard + i);
                break;
            case 4:
                UpdateCellIndex(tmpCell, Cell->xBoard + i, Cell->yBoard - i);
                break;
            case 5://5 - 8 rook
                UpdateCellIndex(tmpCell, Cell->xBoard, Cell->yBoard + i);
                break;
            case 6:
                UpdateCellIndex(tmpCell, Cell->xBoard, Cell->yBoard - i);
                break;
            case 7:
                UpdateCellIndex(tmpCell, Cell->xBoard - i, Cell->yBoard);
                break;
            case 8:
                UpdateCellIndex(tmpCell, Cell->xBoard + i, Cell->yBoard);
                break;
            default:
                break;
        }
        //if there is no one diagonally i steps or there is an enemy
        if (IsEmpty(Board, tmpCell) == 1 || IsTwoCellInSameColor(Board, Cell, tmpCell) == 0) {
            move *Move = InitMoveByBoardIndex(tmpCell->xBoard, tmpCell->yBoard);
            if (Move == NULL) {
                free(tmpCell);
                return 0;
            }
            AddMovesToPieceMoves(result, Move);
            if (IsTwoCellInSameColor(Board, Cell, tmpCell) == 0 && IsEmpty(Board, tmpCell) == 0)
                break;
        } else if (IsTwoCellInSameColor(Board, Cell, tmpCell) == 1)
            break;//can not leap over other peaces
    }
    free(tmpCell);
    return 1;
}
//Queen function: ---------------------------------

/*The function gets a game board and a pointer to a cell
 * (its supposed to be a Queen on that cell). It returns a PieceMoves structure that it
 * cell is copy of that cell, and it moves list
 * field is a list of all available Queen moves that
 * the Queen on this cell can do.
 * If there is any memory error, it returns NULL.
 */

PieceMoves *QueenMoves(char Board[BOARD_SIZE][BOARD_SIZE], cell *Cell) {
    cell *Cell2 = InitCellByBoardIndex(Cell->xBoard, Cell->yBoard);
    PieceMoves *TheRookMoves = RookMoves(Board, Cell);
    PieceMoves *TheBishopMoves = BishopMoves(Board, Cell2);

    if (TheRookMoves == NULL || TheBishopMoves == NULL)
        return NULL;

    AddMovesToPieceMoves(TheRookMoves, TheBishopMoves->listOfMoves);
    free(TheBishopMoves->Cell);
    free(TheBishopMoves);//TheRookMoves.listofmoves shuldnt free
    return TheRookMoves;
}
