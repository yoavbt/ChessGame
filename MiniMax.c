#include "MiniMax.h"




//Initializations: -------------------------------------------------------------------------------------------------


/*The function gets a pointer to PieceMoves struct PM,
 * and creates a pointer to PieceMovesNode struct that
 * contained in his PieceMoves-field a pointer to PM.
 */
PieceMovesNode *InitNode(PieceMoves *PM) {
    PieceMovesNode *result = (PieceMovesNode *) malloc(sizeof(PieceMovesNode));
    if (result == NULL) return NULL;
    result->Next = NULL;
    result->PieceMove = PM;
    return result;
}


/*The function creates a new PieceMovesList struct
 * with NULL fields, and returns a pointer to it.
 */
PieceMovesList *InitList() {
    PieceMovesList *result = (PieceMovesList *) malloc(sizeof(PieceMovesList));
    if (result == NULL) return NULL;
    result->First = NULL;
    return result;
}


//The main functions: -----------------------------------------------------------------------------------------------


/*The function calculates the best move that the AI can do. It gets the current
 * game board and color of the AI player.
 * The function returns a piecemoves struct with one move. the Piecemoves's cell is the
 *source of the move and the move-cell is the destination.
*/
PieceMoves *GetAIMove(char Board[BOARD_SIZE][BOARD_SIZE], char *Color) {
    BestMove = InitPieceMoves(InitCellByBoardIndex(-1, -1));
    if (BestMove == NULL)return NULL;
    if (BestMove->Cell == NULL) {
        FreePieceMoves(BestMove);
        return NULL;
    }
    BestMove->listOfMoves = InitMove(InitCellByBoardIndex(-1, -1));
    if (BestMove->listOfMoves == NULL || BestMove->listOfMoves->Cell == NULL) {
        FreePieceMoves(BestMove);
        return NULL;
    }
    int BestScore = AlphaBeta(Board, Difficulty, -100000000, 100000000, Color);
    if (BestScore == 90909090)
        return NULL;
    return BestMove;
}


/*The function gets a game-board, a tree-depth, the integers alpha and beta and the current player color,
 *uses the alphaBeta-algorithm, and returns the best score that a sequence
 *of depth-moves can bring the game-board.
 * If there is any memory errors - It returns 99999.
 */
int AlphaBeta(char Board[BOARD_SIZE][BOARD_SIZE], int depth,
              int alpha, int beta, char *CurrColor) {
    PieceMovesList *TheBoardMoves = BoardMoves(Board, CurrColor);
    int TmpNum = 0, Islegal;
    if (TheBoardMoves == NULL) return 99999;
    if (depth == 0 || TheBoardMoves->First == NULL) {//If depth = 0 or is a leaf
        FreePieceMovesList(TheBoardMoves);
        return ScoreBoardCal(Board);
    }
    if (depth % 2 == Difficulty % 2) {//If its max node
        int max = -100000000;
        PieceMovesNode *TmpNode = TheBoardMoves->First;
        while (TmpNode != NULL) {
            move *TmpMove = TmpNode->PieceMove->listOfMoves;
            while (TmpMove != NULL) {//This while goes over all the moves that the currColor-player can do.
                char TmpBoard[BOARD_SIZE][BOARD_SIZE];
                CopyBoard(TmpBoard, Board);
                UpdateBoard(TmpBoard, TmpNode->PieceMove->Cell, TmpMove->Cell);
                Islegal = IsMoveLegal(Board, TmpNode->PieceMove->Cell, TmpMove->Cell);
                //If that move is not legal, i.e. it makes the king threatened.
                if (Islegal == 0) {
                    TmpMove = TmpMove->next;//Go on to the next move.
                    continue;
                } else if (Islegal == 2) {//There is memory error
                    FreePieceMovesList(TheBoardMoves);
                    return 99999;
                }
                //recursively, search the best score with that move:
                TmpNum = AlphaBeta(TmpBoard, depth - 1, alpha, beta, RivalColor(CurrColor));
                if (TmpNum == 99999) {//If there was memory problem with any call to the function.
                    FreePieceMovesList(TheBoardMoves);
                    return 99999;
                }
                if (TmpNum >= max) {
                    //If this move is better, and this is the move that the computer should do
                    if (depth == Difficulty && TmpNum > max) {
                        UpdateCellIndex(BestMove->Cell, TmpNode->PieceMove->Cell->xBoard,
                                        TmpNode->PieceMove->Cell->yBoard);
                        UpdateCellIndex(BestMove->listOfMoves->Cell, TmpMove->Cell->xBoard, TmpMove->Cell->yBoard);
                    }
                        /*If it is the root of the minimax-tree and that move just as good as BestMove,
                         * but has better priority than the last-best-move.
                        */
                    else if (depth == Difficulty && TmpNum == max &&
                             IsBetterPriority(TmpNode->PieceMove->Cell, TmpMove->Cell)) {
                        UpdateCellIndex(BestMove->Cell, TmpNode->PieceMove->Cell->xBoard,
                                        TmpNode->PieceMove->Cell->yBoard);
                        UpdateCellIndex(BestMove->listOfMoves->Cell, TmpMove->Cell->xBoard, TmpMove->Cell->yBoard);
                    }
                    max = TmpNum;
                }
                alpha = MAX(alpha, max);
                if (beta <= alpha)
                    break;
                TmpMove = TmpMove->next;
            }
            if (beta <= alpha)
                break;
            TmpNode = TmpNode->Next;
        }
        FreePieceMovesList(TheBoardMoves);
        return max;
    } else {//If it is min-node
        int min = 100000000;
        PieceMovesNode *TmpNode = TheBoardMoves->First;
        while (TmpNode != NULL) {
            move *TmpMove = TmpNode->PieceMove->listOfMoves;
            while (TmpMove != NULL) {//This while goes over all the moves that the currColor-player can do.
                char TmpBoard[BOARD_SIZE][BOARD_SIZE];
                CopyBoard(TmpBoard, Board);
                UpdateBoard(TmpBoard, TmpNode->PieceMove->Cell, TmpMove->Cell);
                Islegal = IsMoveLegal(Board, TmpNode->PieceMove->Cell, TmpMove->Cell);
                //If that move is not legal, i.e. it makes the king threatened.
                if (Islegal == 0) {
                    TmpMove = TmpMove->next;//Go on to the next move.
                    continue;
                } else if (Islegal == 2) {//There is memory error
                    FreePieceMovesList(TheBoardMoves);
                    return 99999;
                }
                //recursively, search the best score with that move:
                TmpNum = AlphaBeta(TmpBoard, depth - 1, alpha, beta, RivalColor(CurrColor));
                if (TmpNum == 99999) {//If there was memory problem with any call to the function.
                    FreePieceMovesList(TheBoardMoves);
                    return 99999;
                }
                min = MIN(min, TmpNum);
                beta = MIN(beta, min);
                if (beta <= alpha)
                    break;
                TmpMove = TmpMove->next;
            }
            if (beta <= alpha)
                break;
            TmpNode = TmpNode->Next;
        }
        FreePieceMovesList(TheBoardMoves);
        return min;
    }
}


//Auxiliary functions: -------------------------------------------------------------------------------------------------

//On moves, pieceMoves etc.: ---------------------------------------------

/*The function gets a pointer to PieceMovesNode, and a pointer to PieceMovesList
 * and add the Node to the end of the list.
 */
void AddNodesToPieceMovesList(PieceMovesList *List, PieceMovesNode *Node) {
    if (List != NULL && List->First != NULL) {
        PieceMovesNode *last = List->First;
        while (last->Next != NULL) last = last->Next;//Now last is the last node in the list
        last->Next = Node;
    }
    if (List != NULL && List->First == NULL)
        List->First = Node;
}


//On game-boards : ---------------------------------------------

/*The function gets a pointers to two cells - a move from the From cell to the To cell.
 * It return 1 if that move has lower: source column, source row, destination column, destination row (in this order)
 * than those BestMove's paramters.
 * Otherwise, it returns 0.
 */
int IsBetterPriority(cell *From, cell *To) {
    if (BestMove->Cell->xBoard == -1)
        return 1;

    if (From->yBoard > BestMove->Cell->yBoard)
        return 0;
    if (From->yBoard < BestMove->Cell->yBoard)
        return 1;

    if (From->xUser > BestMove->Cell->xUser)
        return 0;
    if (From->xUser < BestMove->Cell->xUser)
        return 1;

    if (To->yBoard > BestMove->listOfMoves->Cell->yBoard)
        return 0;
    if (To->yBoard < BestMove->listOfMoves->Cell->yBoard)
        return 1;

    if (To->xUser > BestMove->listOfMoves->Cell->xUser)
        return 0;
    if (To->xUser < BestMove->listOfMoves->Cell->xUser)
        return 1;

    return 0;//its the same move
}


/*The function gets a game-board and a player color(black or white)
 * and returns a pieceMovesList struct that contains all the posible moves that the
 * player with that color can do.
 * If there is memory error, it returns NULL.
 */
PieceMovesList *BoardMoves(char Board[BOARD_SIZE][BOARD_SIZE], char *Color) {
    PieceMovesList *result = InitList();
    int col , row;
    if (result == NULL) return NULL;
    for (col = 0; col < BOARD_SIZE; col++) {
        for (row = 0; row < BOARD_SIZE; row++) {
            if (Board[row][col] != EMPTY && strcmp(GetPieceColor(Board[row][col]), Color) == 0) {
                cell *CurrCell = InitCellByBoardIndex(row, col);
                if (CurrCell == NULL) {
                    FreePieceMovesList(result);
                    return NULL;
                }
                PieceMoves *PM = GetPieceMoves(Board, CurrCell);
                if (PM == NULL) {
                    FreePieceMovesList(result);
                    free(CurrCell);
                    return NULL;
                }
                if (PM->listOfMoves != NULL)
                    AddNodesToPieceMovesList(result, InitNode(PM));
            }
        }
    }
    return result;
}


/*The Function gets a game board and calculates the sum of the white piece
 * and the sum of the black pieces(pawn - 1, bishop - 3, knight - 3, rook - 5, queen - 9, king - 100)
 * and return the (black-sum - white-sum) if the ComputerColor is black,
 * or (white-sum - black-sum) if the ComputerColor is white.
 */
int ScoreBoardCal(char Board[BOARD_SIZE][BOARD_SIZE]) {
    int BlackSum = 0, WhiteSum = 0;
    int i , j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (Board[i][j] == WHITE_PAWN)
                WhiteSum += 1;
            else if (Board[i][j] == BLACK_PAWN)
                BlackSum += 1;

            else if (Board[i][j] == WHITE_BISHOP)
                WhiteSum += 3;
            else if (Board[i][j] == BLACK_BISHOP)
                BlackSum += 3;

            else if (Board[i][j] == WHITE_KNIGHT)
                WhiteSum += 3;
            else if (Board[i][j] == BLACK_KNIGHT)
                BlackSum += 3;

            else if (Board[i][j] == WHITE_ROOK)
                WhiteSum += 5;
            else if (Board[i][j] == BLACK_ROOK)
                BlackSum += 5;

            else if (Board[i][j] == WHITE_QUEEN)
                WhiteSum += 9;
            else if (Board[i][j] == BLACK_QUEEN)
                BlackSum += 9;

            else if (Board[i][j] == WHITE_KING)
                WhiteSum += 100;
            else if (Board[i][j] == BLACK_KING)
                BlackSum += 100;
        }
    }
    if (strcmp(CompColor, BLACK) == 0)
        return BlackSum - WhiteSum;
    else
        return WhiteSum - BlackSum;
}


//Memory release: ---------------------------------------------------------------------------------------------------


/*The function gets a pointer to PieceMovesNode, and free it with all his allocated memory.
 */
void FreePieceMovesNode(PieceMovesNode *PMNode) {
    if (PMNode == NULL)return;
    PMNode->Next = NULL;
    FreePieceMoves(PMNode->PieceMove);
    free(PMNode);
}


/*The function gets a pointer to PieceMovesList, and free it with all his allocated memory.
 */
void FreePieceMovesList(PieceMovesList *PMList) {
    PieceMovesNode *tmp1 = PMList->First;
    PieceMovesNode *tmp2;
    while (tmp1 != NULL) {
        tmp2 = tmp1->Next;
        FreePieceMovesNode(tmp1);
        tmp1 = tmp2;
    }
    free(PMList);
}
