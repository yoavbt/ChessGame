#include "Undo.h"

//Initializations and updates: -------------------------------------------------------------------------------------
//Variables: ------------------------------------------------------------------------------------------------------

char MovesHistory[7][BOARD_SIZE][BOARD_SIZE];//7 boards - 1 for the current mode, and 3 for each player's last moves
cell *MovesCells[7][2];//From-cell and To-cell for each move


/*The function initialize the boards and
 * the cell's info for no info - X's for the boards and
 * invalid info for the cells(-1)
 */
void InitMovesHistory() {
    int i;
    for (i = 0; i < 7; i++) {
        PutXBoard(MovesHistory[i]);
        MovesCells[i][0] = InitCellByBoardIndex(-1, -1);
        MovesCells[i][1] = InitCellByBoardIndex(-1, -1);
    }
    CopyBoard(MovesHistory[6], Board);
}


/*The function got a game board and put X's in all cells
 */
void PutXBoard(char CurrBoard[BOARD_SIZE][BOARD_SIZE]) {
    int row, col;
    for (row = 0; row < BOARD_SIZE; row++)
        for (col = 0; col < BOARD_SIZE; col++)
            CurrBoard[row][col] = 'x';
}


/*The function get a current game board and two cells -
 * from where the move done and it destination.
 * The function updates the data and push the board and the cells
 * to the last place in the array.
 * The function pop the first board and cells - in place 0 in the arrays.
 */
void UpdateMovesHistory(char newMove[BOARD_SIZE][BOARD_SIZE], cell *from, cell *to) {
    int i;
    for (i = 0; i < 6; i++) {
        CopyBoard(MovesHistory[i], MovesHistory[i + 1]);
        UpdateCellIndex(MovesCells[i][0], MovesCells[i + 1][0]->xBoard, MovesCells[i + 1][0]->yBoard);
        UpdateCellIndex(MovesCells[i][1], MovesCells[i + 1][1]->xBoard, MovesCells[i + 1][1]->yBoard);
    }
    CopyBoard(MovesHistory[6], newMove);
    UpdateCellIndex(MovesCells[6][0], from->xBoard, from->yBoard);
    UpdateCellIndex(MovesCells[6][1], to->xBoard, to->yBoard);
}

//The Undo function and auxiliary functions: ------------------------------------------------------------------------


/*The function pop the last game board from the Moves history.
 * if there is no game board to pop. it prints EMPTY_HISTORY message.
 * If not, the function prints the updated game board.
 * If there is one last move in the history - it return to it,
 * if there is two - it return to the second game board from the end.
 */
void UndoLastMove() {
    if (MovesHistory[5][0][0] == 'x') {
        printf(EMPTY_HISTORY);
        return;
    } else {
        CurrPlayerColor = (strcmp(CurrPlayerColor, BLACK) == 0 ? WHITE : BLACK);
        popLastMove(CurrPlayerColor);
        if (MovesHistory[5][0][0] != 'x') {//if there is another game board to return it.
            CurrPlayerColor = (strcmp(CurrPlayerColor, BLACK) == 0 ? WHITE : BLACK);
            popLastMove(CurrPlayerColor);
        }
    }
    PrintBoard();
}


/*The function take of the last game board in the MovesHistory array,
 * copy it to the Board, and put X's on the first place game board.
 * It works like a queue - the most recent go out, and X's got to the struct.
 * The function got a color to print the appropriate message.
 */
void popLastMove(char *WhoPlayedLast) {
    int i;
    printUndo(WhoPlayedLast);
    for (i = 5; i >= 0; i--) {
        CopyBoard(MovesHistory[i + 1], MovesHistory[i]);
        UpdateCellIndex(MovesCells[i + 1][0], MovesCells[i][0]->xBoard, MovesCells[i][0]->yBoard);
        UpdateCellIndex(MovesCells[i + 1][1], MovesCells[i][1]->xBoard, MovesCells[i][1]->yBoard);
    }
    PutXBoard(MovesHistory[0]);
    UpdateCellIndex(MovesCells[0][0], -1, -1);
    UpdateCellIndex(MovesCells[0][1], -1, -1);
    CopyBoard(Board, MovesHistory[6]);
}


/*The function got a color and prints the appropriate undo message,
 * from the cells arrays - one from the To-Cell array : MovesCells[.][1]
 * and one from the From-Cell array : MovesCells[.][0]
 */
void printUndo(char *WhoPlayedLast) {
    printf("Undo move for %s player: ", WhoPlayedLast);
    PrintCell(MovesCells[6][1], 0, 0);//print the "to-cell"
    printf(" -> ");
    PrintCell(MovesCells[6][0], 0, 0);//print the "from-cell"
    printf("\n");
}

/*The function free all memory related to MoveHistory
 */
void FreeMoveHistory() {
    int i;
    for (i = 0; i < 7; i++) {
        free(MovesCells[i][0]);
        free(MovesCells[i][1]);
    }
}
