#include "GameState.h"

/*The function manage the game - it changes
 * the turns between the user to the other user
 * (or computer - depends on GameMode).
 * It returns:
 * 1 if there was a quit request, a draw or a winning.
 * 2 if there was a memory error.
 * 3 if there was a reset request.
 */

int GameState() {
    int TurnResult, gameStatus;
    while (1) {
        if (GameMode == 1) {
            if (strcmp(CurrPlayerColor, UserColor) == 0) {
                TurnResult = UserTurn();
            }
            else {
                TurnResult = ComputerTurn();
            }
        }
        else {
            TurnResult = UserTurn();
        }
        if (TurnResult == 1) return 1; // if quit game cmd
        else if (TurnResult == 2) return 2; // if memory problem
        else if (TurnResult == 3) return 3; // if reset
        gameStatus = GameStatus(CurrPlayerColor); // 1 if end of game  2 if mem problem
        switch (gameStatus) {
            case 0: //All good
                break;
            case 1: //Tie
                printf(DRAW);
                return 1;
            case 2: // mem alloc error
                return 0;
            case 3: // Check
                printf(CHECK, (strcmp(CurrPlayerColor, BLACK) == 0 ? WHITE : BLACK));
                break;
            case 4: //Mate
                printf(MATE, CurrPlayerColor);
                return 1;
            default:
                break;
        }
        CurrPlayerColor = (strcmp(CurrPlayerColor, BLACK) == 0 ? WHITE : BLACK);
    }
}


/* The function Handles user turn and updates the game board.
 * It return:
 * 0 if its the end of the user Turn without problems
 * 1 it there is a quit request
 * 2 if there is memory allocation error
 * 3 if there is a reset Game request
 */


int UserTurn() {
    int ParsingResult;
    ParsingResult = -1;
    PrintBoard();
    while (ParsingResult == -1) {
        printf("Enter your move (%s player):\n", CurrPlayerColor);
        GetInput();
        ParsingResult = ParsingUserTurn();
    }
    return ParsingResult;
}

/* The function Parses input string entered by user at user turn and handles each case
 * It returns:
 * -1 if the User wanted to get_moves, get_best_moves, get_score, save (user's turn hasn't ended)
 * 0 if its the End of user turn (the user made a move)
 * 1 if the User wants to quit
 * 2 if there is a Memory allocation error
 * 3 if the user want to reset the Game
 */

int ParsingUserTurn(void) {
    // At most 3 arguments for Move cmd
    char *Cmd;
    char *Arg1;
    char *Arg2;
    char *Arg3;
    Cmd = strtok(Input, " ");
    if (Cmd == NULL) {
        printf(INVALID_CMD);
        return -1;
    }
    if (strcmp(Cmd, "quit") == 0) {
        printf(EXIT_CMD);
        return 1;
    } else if (strcmp(Cmd, "reset") == 0) {
        InitBoard();
        CurrPlayerColor = WHITE;
        printf(RESET_CMD);
        return 3;
    } else if (strcmp(Cmd, "get_moves") == 0) {
        Arg1 = strtok(NULL, " ");
        if (Arg1 == NULL) {
            printf(INVALID_CMD);
            return -1;
        }
        if (CheckMoveSyntx(Arg1)) {
            if (CheckMoveCmd(Arg1)) {
                return GetMove(Arg1);
            }
            printf(INV_POS);
            return -1;
        }
        printf(INVALID_CMD);
        return -1;
    } else if (strcmp(Cmd, "move") == 0) {
        Arg1 = strtok(NULL, " ");
        if (Arg1 == NULL) {
            printf(INVALID_CMD);
            return -1;
        }
        Arg2 = strtok(NULL, " ");
        if (Arg2 == NULL) {
            printf(INVALID_CMD);
            return -1;
        }
        Arg3 = strtok(NULL, " ");
        if (Arg3 == NULL) {
            printf(INVALID_CMD);
            return -1;
        }
        if (CheckMoveSyntx(Arg1) && CheckMoveSyntx(Arg3) && CheckToCmd(Arg2)) {
            if (CheckMoveCmd(Arg1) && CheckMoveCmd(Arg3)) {
                return SetMove(Arg1, Arg3);
            } else {
                printf(INV_POS);
                return -1;
            }
        }
        printf(INVALID_CMD);
        return -1;
    } else if (strcmp(Cmd, "save") == 0) {
        Arg1 = strtok(NULL, " ");
        if (Arg1 == NULL) {
            printf(INVALID_CMD);
            return -1;
        }
        if (!SaveCurrGame(Arg1)) {
            printf(SAVE_ERR);
        } else printf(SAVE_SUCC, Arg1);
        return -1;
    }
     else if (strcmp(Cmd, "undo") == 0) {
        UndoLastMove();
        return -1;
    }
    else {
        printf(INVALID_CMD);
        return -1;
    }

}


/*The function calculates the best move that the computer
 * can do, using the Minimax algorithm, and concidering
 * the difficulty level, and plays that move on the
 * game board.
 * It returns 0 if all good, and 2 if there is a memory error.
 */
int ComputerTurn() {
    PieceMoves *AITurn = GetAIMove(Board, CurrPlayerColor);
    if (AITurn == NULL)
        return 2;
    char *PieceType = GetType(Board[AITurn->Cell->xBoard][AITurn->Cell->yBoard]);
    UpdateBoard(Board, AITurn->Cell, AITurn->listOfMoves->Cell);
    printf(AI_TURN, PieceType, AITurn->Cell->xUser, AITurn->Cell->yUser, AITurn->listOfMoves->Cell->xUser,
           AITurn->listOfMoves->Cell->yUser);
    UpdateMovesHistory(Board, AITurn->Cell, AITurn->listOfMoves->Cell);
    FreePieceMoves(AITurn);
    return 0;
}


/*The function handles the get_moves request.
 * It prints the available moves from the cell
 * written in the Arg1.
 * It returns -1 if all done good or the command
 * is not in the right syntax.
 * If there is memory error, it returns 2.
 */

int GetMove(char *Arg1) {
    cell *CurrCell;
    cell *CopyCell;
    PieceMoves *MoveList;
    CurrCell = InitCell(Arg1);
    int Mem;
    if (CurrCell == NULL) {
        return 2;
    }
    if (!IsValidPos(CurrCell)) {
        printf(INV_POS);
    } else if (!IsCellContainPiece(Board, CurrCell)) {
        printf(NOT_CON_PLAYER_PIECE);
    } else {
        CopyCell = InitCellByBoardIndex(CurrCell->xBoard, CurrCell->yBoard);
        MoveList = GetPieceMoves(Board, CopyCell);
        Mem = PrintPieceMoves(MoveList);
        if (Mem == 2) {
            free(CurrCell);
            FreePieceMoves(MoveList);
            return 2;
        }
        FreePieceMoves(MoveList);
    }
    free(CurrCell);
    return -1;
};


/*The function handles the request for move.
 * It gets two strings, contained cells - the source
 * piece to move and it destination.
 * If that move is valid, it updates the board and return 0.
 * If the command is wrong or the move is unavailable, it returns -1.
 * If there was a memory error, it returns 2.
 */


int SetMove(char *Arg1, char *Arg3) {
    PieceMoves *validMoves;
    move *temp;
    cell *copyOfFrom;
    cell *from;
    cell *to;
    int isKingThreatened;
    int isKingWillNotBeThreatened;
    from = InitCell(Arg1);
    if (from == NULL) return 2;
    to = InitCell(Arg3);
    if (to == NULL) {
        free(from);
        return 2;
    }
    if (!IsValidPos(from) || !IsValidPos(to)) {
        free(to);
        free(from);
        printf(INV_POS);
        return -1;
    }
    if (strcmp(GetType(GetPiece(Board, from)), EMPTY_STRING) == 0
        || strcmp(GetPieceColor(GetPiece(Board, from)), CurrPlayerColor) != 0) {
        printf(NOT_CON_YOUR_PIECE);
        free(to);
        free(from);
        return -1;
    }
    isKingThreatened = IsKingThreatend(Board, CurrPlayerColor);
    if (isKingThreatened == 2) {
        free(to);
        free(from);
        return 2;
    }
    copyOfFrom = InitCellByBoardIndex(from->xBoard, from->yBoard);
    if (copyOfFrom == NULL) {
        free(to);
        free(from);
        return 2;
    }
    validMoves = GetPieceMoves(Board, copyOfFrom);
    if (validMoves == NULL) {
        free(to);
        free(from);
        free(copyOfFrom);
        return 2;
    }
    temp = validMoves->listOfMoves;
    while (temp != NULL) {
        if (IsSameCell(temp->Cell, to)) {
            isKingWillNotBeThreatened = IsMoveLegal(Board, copyOfFrom, to);
            if (isKingWillNotBeThreatened == 2) {
                free(to);
                free(from);
                FreePieceMoves(validMoves);
                return 2;
            } else if (isKingWillNotBeThreatened == 0) {
                if (isKingThreatened == 1) {
                    printf(KING_STILL_THREATENED);
                } else {
                    printf(KING_WILL_BE_THREATENED);
                }
                free(to);
                free(from);
                FreePieceMoves(validMoves);
                return -1;
            } else {
                UpdateBoard(Board, from, to);
                UpdateMovesHistory(Board, from, to);
                free(to);
                free(from);
                FreePieceMoves(validMoves);
                return 0;
            }

        }
        temp = temp->next;
    }
    printf(ILL_MOV);
    free(to);
    free(from);
    FreePieceMoves(validMoves);
    return -1;
};

/**
 * param: String To
 * return : 1 if To == "to" else 0
 *  action: check that a String equal to "to"
 */

int CheckToCmd(char *To) {
    if (strcmp(To, "to") == 0)return 1;
    return 0;
}


/**
 * param: String Arg
 * return : 1 iff Arg is a string that the first char is '<' last is '>'
 *  and there is ',' somewhere in between
 *  action: check the correct form of <,>
 */

int CheckMoveSyntx(char *Arg) {
    char c;
    int i;
    if (Arg[0] != '<' || Arg[strlen(Arg) - 1] != '>') return 0;
    i = 0;
    while (i < strlen(Arg) - 1) {
        i++;
        c = Arg[i];
        if (c == ',') return 1;
    }
    return 0;
}

/**
 * param: String Arg
 * return : 1 iff <X,Y> if x is a number and Y is a string of size 1 exactly
 *  action: check the correct form of <,> to be able to create cell
 */
int CheckMoveCmd(char *Arg) {
    char d[100];
    strcpy(d, Arg);
    char *x;
    char *y;
    x = strtok(d, "<,>");
    if (x == NULL || atoi(x) == 0) return 0;
    y = strtok(NULL, "<,>");
    if (y == NULL || strlen(y) > 1) return 0;
    return 1;
}

/* The function gets a game board and last player color, and return status (of enemy) after move
 * It returns:
 * 0 if there is Noting special (go to next turn)
 * 1 if there is a Tie
 * 2 if there is a Memory allocation error
 * 3 if there is a Check
 * 4 if there is a Mate
 * */


int GameStatus(char *LastPlayer) {
    char *NextPlayer = strcmp(LastPlayer, WHITE) == 0 ? BLACK : WHITE;
    int Check, Mate;
    Check = IsKingThreatend(Board, NextPlayer);
    Mate = NoMoreMoves(Board, NextPlayer);
    if (Check == 2 || Mate == 2) return 2;
    if (Check == 1) {
        if (Mate == 1) return 4;
        return 3;
    } else {
        if (Mate == 1) return 1;
        return 0;
    }
}